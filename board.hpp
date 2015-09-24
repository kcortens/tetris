#ifndef BOARD_HPP
#define BOARD_HPP

#include <list>
#include <vector>

#include <glm/glm.hpp>

#include "block.hpp"
#include "piece.hpp"
#include "grid.hpp"

class Board {
    public:
        Board();
        void draw(glm::mat4* projection, GLuint MVPID, GLuint colourID);
        bool isColliding(Piece *active_piece);
        void insertPiece(Piece *active_piece);
    private:
        void buildVerts();
        void calcLineCount();
        void removeFullLines();
        Grid* board_grid;
        std::vector<Block**> cur_board;
        std::vector<int> board_line_count;
        static const int vert_array_size = 20*10*2;
        GLuint vbuf, vbo, colour_buffer;
        std::vector<glm::vec2> vert_buffer;
        unsigned short indices[(2*11*20) + (2*21*10)];
};

#endif
