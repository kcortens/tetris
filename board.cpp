#include <stdio.h>

#include "board.hpp"

//GLfloat Board::vert_buffer_data[20*10*2];
const int Board::vert_array_size;

Board::Board()
{
    Block** new_row;

    for(int i=0; i<20; i++)
    {
        new_row = new Block*[10];
        for(int j=0;j<10;j++) new_row[j] = NULL;
        cur_board.push_back(new_row);

        board_line_count.push_back(0);
    }

    buildVerts();

    glGenBuffers(1, &vbuf);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &colour_buffer);

    board_grid = new Grid(10,20,16,0.f,0.f);
}

bool Board::isColliding(Piece *active_piece)
{
    bool result = false;
    std::vector<glm::vec2> active_pos = active_piece->getPos();
    glm::vec2 next_pos;

    for (int i=0;i<4;i++)
    {
        next_pos = glm::vec2( active_pos[i][0], active_pos[i][1]+1 );
        if(next_pos[1] > 19) result = true;
        else
        {
            Block** next_row = cur_board[next_pos[1]];
            Block* next_block = next_row[static_cast<int>(next_pos[0])];

            if(next_block != NULL)
                result = true;
        }
    }

    return result;
}

void Board::insertPiece(Piece *active_piece)
{
    Block* new_block;
    Block** ins_row;
    std::vector<glm::vec2> final_pos = active_piece->getPos();
    glm::vec3 block_colour = active_piece->getColour();

    for(int i=0;i<4;i++)
    {
        new_block = new Block(16, block_colour[0], block_colour[1], block_colour[2]);
        new_block->setPos(final_pos[i][0], final_pos[i][1]);
        ins_row = cur_board[static_cast<int>(final_pos[i][1])];
        ins_row[static_cast<int>(final_pos[i][0])] = new_block;
    }

    Board::calcLineCount();
    Board::removeFullLines();
}

void Board::calcLineCount()
{
    int count;

    for (int i = 0; i < 20; ++i)
    {
        count = 0;
        for (int j = 0; j < 10; ++j)
        {
            if( cur_board[i][j] != NULL ) count++;
        }
        board_line_count[i] = count;
    }
}

void Board::removeFullLines()
{
    Block** new_row;

    for (int i = 0; i < 20; ++i)
    {
        if( board_line_count[i] == 10 )
        {
            cur_board.erase(cur_board.begin() + i);

            //fprintf(stdout, "i:%d\n", i);

            new_row = new Block*[10];
            for(int j=0;j<10;j++) new_row[j] = NULL;
            cur_board.insert(cur_board.begin(), new_row);
        }
    }
}

void Board::buildVerts()
{
    for(int i=0;i<231;i++)
        vert_buffer.push_back(glm::vec2( ((i % 11) * 16.f), (( i / 11 ) * 16.f) ));

    int count = 0;
    for(int y=0;y<21;y++)
    {
        for(int x=0;x<10;x++)
        {
            indices[count++] = y*11 + x;
            indices[count++] = y*11 + x + 1;
        }
    }

    for(int x=0;x<11;x++)
    {
        for(int y=0;y<20;y++)
        {
            indices[count++] = y*11 + x;
            indices[count++] = (y+1) * 11 + x;
        }
    }
}

void Board::draw(glm::mat4* projection, GLuint MVPID, GLuint colourID)
{
    glm::vec3 x = glm::vec3( 0.f, 0.f, 0.f );

    Block** cur_row;
    Block* cur_block;

    for(int i=0;i<20;i++)
    {
        cur_row = cur_board[i];

        for(int j=0;j<10;j++)
        {
            cur_block = cur_row[j];
            if(cur_block != NULL) cur_block->draw(projection, MVPID, colourID, glm::vec2(j,i));
        }
    }

    board_grid->draw(projection, MVPID, colourID);
}
