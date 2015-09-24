#ifndef GRID_HPP
#define GRID_HPP

#include <vector>

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class Grid {
    public:
        Grid(int width, int height, float size, float x_pos, float y_pos);
        void draw(glm::mat4* projection, GLuint MVPID, GLuint colourID);
    private:
        int vert_count, index_count;
        float x_0, y_0, block_width;
        std::vector<glm::vec2> vert_buffer;
        unsigned short *indexes;
        GLuint vbuf, vbo, colour_buffer;
        std::vector<unsigned short> indices;
};

#endif
