#include <stdio.h>
#include <vector>
#include <stdlib.h>
#include <time.h>

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.hpp"
#include "block.hpp"
#include "piece.hpp"
#include "board.hpp"
#include "grid.hpp"

Piece* active_piece;
Piece* next_piece;
Piece* hold_piece;
float drop_speed = .5;

void keyCB( GLFWwindow* window, int key, int scancode, int action, int mod )
{
    if ( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS )
        glfwSetWindowShouldClose(window, GL_TRUE);
    else if ( key == GLFW_KEY_LEFT && action == GLFW_PRESS )
        active_piece->shift(false);
    else if ( key == GLFW_KEY_RIGHT && action == GLFW_PRESS )
        active_piece->shift(true);
    else if ( key == GLFW_KEY_UP && action == GLFW_PRESS )
        active_piece->rotate(true);
    else if ( key == GLFW_KEY_DOWN )
    {
        if( action == GLFW_PRESS ) drop_speed /= 4;
        else if ( action == GLFW_RELEASE ) drop_speed *= 4;
    }
    else if ( key == GLFW_KEY_SPACE && action == GLFW_PRESS )
    {
        Piece* tmp_piece;
        if( hold_piece == NULL )
        {
            tmp_piece = next_piece;
            next_piece = new Piece( static_cast<PieceName>(1) );
            next_piece->setNext();
        }
        else
            tmp_piece = hold_piece;
        tmp_piece->setActive();

        hold_piece = active_piece;
        hold_piece->setHold();

        active_piece = tmp_piece;
    }
}

double calcFPS(double *prev_time, int *frame_count)
{
    static double cur_time;
    static double fps = 0;
    cur_time = glfwGetTime();

    if(cur_time - *prev_time > 1)
    {
        fps = *frame_count / ( (cur_time - *prev_time) / 1.f );
        *prev_time = cur_time;
        *frame_count = 0;
    }

    return fps;
}

void render(GLuint *vbuf, glm::mat4 mvp, GLuint MVPID)
{
    glClear(GL_COLOR_BUFFER_BIT);

    glUniformMatrix4fv(MVPID, 1, GL_FALSE, &mvp[0][0]);

    //glMatrixMode(GL_MODELVIEW);
    //glLoadIdentity();

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,*vbuf);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableVertexAttribArray(0);


    glBegin(GL_LINES);

    glColor3ub(255,0,0);
    glVertex2f(0.f,0.f);

    glColor3ub(0,0,255);
    glVertex2f(800.f,600.f);

    glEnd();

    glFlush();
}

void initGL()
{
    glViewport(0,0, 800, 600);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 800, 600, 0, 0, 1);
}

int main(int argc, char** argv)
{
    glm::mat4 model, view, projection, mvp;
    srand(time(NULL));

    if (!glfwInit())
    {
        fprintf(stderr, "glfw init fail");
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    GLFWwindow* window = glfwCreateWindow(800, 600, "butts", NULL, NULL);
    glfwMakeContextCurrent(window);
    if(glewInit() != GLEW_OK)
    {
        fprintf(stderr, "glew init fail");
        return 1;
    }

    glClearColor(.7,.7,.7,1.f);

    //glEnable(GL_CULL_FACE);

    //initGL();

    glfwSetKeyCallback(window, keyCB);
    GLuint shaderID = LoadShaders("vshader.vsr", "fshader.fsr");
    GLuint MVPID = glGetUniformLocation(shaderID, "MVP");
    GLuint colourID = glGetUniformLocation(shaderID, "vertColour");

    projection = glm::ortho(0.f,800.f,600.f, 0.f);
    //projection = glm::perspective(45.f, 4.f/3.f, .1f, 100.f);
    view = glm::lookAt(glm::vec3(4,3,3), glm::vec3(0,0,0), glm::vec3(0,1,0));
    model = glm::mat4(1.0f);
    mvp = projection * view * model;

    static const GLfloat vert_buffer_data[] = {
        200.f,200.f,
        600.f,200.f,
        400.f,500.f,
    };

    int new_piece_rand = rand() % 7;

    Board* game_board = new Board();
    active_piece = new Piece( static_cast<PieceName>(new_piece_rand) );
    active_piece->setActive();
    next_piece = new Piece( static_cast<PieceName>(new_piece_rand) );
    next_piece->setNext();

    double prev_fps_time = glfwGetTime();
    double prev_drop_time = glfwGetTime();
    double cur_drop_time;
    double fps;
    int frame_count = 0;

    Grid *next_grid = new Grid(5, 5, 16, 12.f, 5.f);
    Grid *hold_grid = new Grid(5, 5, 16, 12.f, 13.f);

    while(!glfwWindowShouldClose(window))
    {
        frame_count++;
        fps = calcFPS(&prev_fps_time, &frame_count);

        cur_drop_time = glfwGetTime();
        if(cur_drop_time - prev_drop_time > drop_speed)
        {
            if(!game_board->isColliding(active_piece))
            {
                active_piece->drop();
            }

            else
            {
                game_board->insertPiece(active_piece);
                new_piece_rand = rand() % 7;
                active_piece = next_piece;
                active_piece->setActive();
                next_piece = new Piece( static_cast<PieceName>(new_piece_rand) );
                next_piece->setNext();
                fprintf(stdout, "%d\n", new_piece_rand);
            }

            prev_drop_time = cur_drop_time;
        }

        glUseProgram(shaderID);
        glClear(GL_COLOR_BUFFER_BIT);

        active_piece->draw(&projection, MVPID, colourID);
        next_piece->draw(&projection, MVPID, colourID);
        game_board->draw(&projection, MVPID, colourID);
        if(hold_piece!= NULL) hold_piece->draw(&projection, MVPID, colourID);
        next_grid->draw(&projection, MVPID, colourID);
        hold_grid->draw(&projection, MVPID, colourID);

        glDisableVertexAttribArray(0);

        glFlush();

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
