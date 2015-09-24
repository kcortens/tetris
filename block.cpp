#include <stdio.h>

#include "block.hpp"

const GLfloat Block::vert_buffer_data[] = {
    0.f, 0.f,
    1.f, 0.f,
    0.f, 1.f,
    1.f, 1.f,
};

Block::Block(float block_size) {
    init(block_size);
}

Block::Block(float block_size, float R, float G, float B)
{
    init(block_size);
    colour = glm::vec3(R,G,B);
}

void Block::init(float block_size)
{
    model = glm::mat4(1.f);
    scalar = block_size;
    x_pos = 0;
    y_pos = 0;
    glGenBuffers(1, &vbuf);
}

void Block::draw(glm::mat4* projection, GLuint MVPID, GLuint colourID, glm::vec2 pos)
{
    glm::mat4 mvp;

    glm::mat4 tmp_model = glm::mat4(1.f);
    tmp_model = glm::scale( tmp_model, glm::vec3(scalar, scalar, 0) );
    tmp_model = glm::translate( tmp_model, glm::vec3(pos[0], pos[1], 0) );
    mvp = *projection * tmp_model;

    glUniformMatrix4fv(MVPID, 1, GL_FALSE, &mvp[0][0]);
    glUniform3fv(colourID, 1, &colour[0]);

    glBindBuffer(GL_ARRAY_BUFFER,vbuf);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vert_buffer_data), vert_buffer_data, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glDisableVertexAttribArray(0);
}

void Block::transform()
{
    model = glm::mat4(1.f);
    model = glm::scale( model, glm::vec3(scalar, scalar, 0) );
    model = glm::translate( model, glm::vec3(x_pos, y_pos, 0) );
}

int Block::vertSize()
{
    return sizeof(vert_buffer_data);
}

void Block::move(float x, float y)
{
    x_pos += x;
    y_pos += y;
}

glm::mat4* Block::getModel()
{
    model = glm::mat4(1.f);
    model = glm::translate( model, glm::vec3(x_pos, y_pos, 0) );
    model = glm::scale( model, glm::vec3(scalar, scalar, 0) );

    return &model;
}

void Block::setPos(float x, float y)
{
    x_pos = x;
    y_pos = y;
}
