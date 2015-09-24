#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Block {
    public:
        Block(float block_size);
        Block(float block_size, float R, float G, float B);
        void draw(glm::mat4* projection, GLuint MVPID, GLuint colourID, glm::vec2 pos);
        const static GLfloat* getVerts() { return vert_buffer_data; }
        int vertSize();
        void move(float x, float y);
        void setPos(float x, float y);
        glm::vec2 getPos() {return glm::vec2(x_pos, y_pos);}
        glm::mat4* getModel();
    private:
        void transform();
        void init(float block_size);
        float scalar;
        float x_pos;
        float y_pos;
        glm::vec3 colour;
        GLuint vbuf;
        static const GLfloat vert_buffer_data[];
        glm::mat4 model;
};

#endif
