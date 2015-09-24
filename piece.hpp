#ifndef PIECE_HPP
#define PIECE_HPP

#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "block.hpp"

enum PieceName { PIECE_I, PIECE_J, PIECE_L, PIECE_O, PIECE_S, PIECE_T, PIECE_Z };

class Piece {
    public:
        Piece(PieceName type);
        void setActive();
        void setNext();
        void setHold();
        void draw(glm::mat4* projection, GLuint MVPID, GLuint colourID);
        void shift(bool right);
        void move(float x, float y);
        void rotate(bool cw);
        void drop();
        glm::vec3 getColour();
        std::vector<glm::vec2> getPos();
    private:
        void setOrigin();
        Block** blocks;
        std::vector<glm::vec2> block_pos;
        PieceName piece_type;
        static const glm::vec3 piece_colours[];
};

#endif
