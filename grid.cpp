#include "grid.hpp"

#include <stdio.h>

Grid::Grid(int width, int height, float size, float x_pos, float y_pos)
{
    vert_count = (width+1) * (height+1);
    for (int i = 0; i < vert_count; ++i)
    {
        vert_buffer.push_back(glm::vec2( ((i % (width+1))), (( i / (width+1) )) ));
    }

    index_count = ((height+1) * width * 2 ) + (height * (width+1) * 2);
    indexes = new unsigned short[index_count];
    int count = 0;
    for(int y = 0; y < height+1; ++y)
    {
        for(int x = 0; x < width; ++x)
        {
            indexes[count++] = (y*(width+1) + x);
            indexes[count++] = (y*(width+1) + x + 1);
        }
    }

    for(int x = 0; x < width+1; ++x)
    {
        for(int y = 0; y < height; ++y)
        {
            indexes[count++] = (y*(width+1) + x);;
            indexes[count++] = ((y+1) * (width+1) + x);
        }
    }

    glGenBuffers(1, &vbuf);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &colour_buffer);

    x_0 = x_pos;
    y_0 = y_pos;
    block_width = size;
}

void Grid::draw(glm::mat4* projection, GLuint MVPID, GLuint colourID)
{
    glm::vec3 x = glm::vec3( 0.f, 0.f, 0.f );
    glm::mat4 mvp;

    glm::mat4 model = glm::mat4(1.f);
    model = glm::scale( model, glm::vec3(block_width, block_width, 0) );
    model = glm::translate( model, glm::vec3(x_0, y_0, 0) );
    mvp = *projection * model;
    glUniformMatrix4fv(MVPID, 1, GL_FALSE, &mvp[0][0]);
    glUniform3fv(colourID, 1, &x[0]);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbuf);
    glBufferData(GL_ARRAY_BUFFER, vert_count*sizeof(glm::vec2), &vert_buffer[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_count*sizeof(unsigned short), &indexes[0], GL_STATIC_DRAW);
    glDrawElements(GL_LINES, index_count, GL_UNSIGNED_SHORT, (void*)0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}
