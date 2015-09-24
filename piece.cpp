#include <stdio.h>

#include "piece.hpp"

int block_size = 1;

const glm::vec3 Piece::piece_colours[] = {
    glm::vec3(0.f, 1.f, 1.f),
    glm::vec3(0.f, 0.f, 1.f),
    glm::vec3(1.f, .5f, 0.f),
    glm::vec3(1.f, 1.f, 0.f),
    glm::vec3(.5f, 1.f, 0.f),
    glm::vec3(.5f, 0.f, .5f),
    glm::vec3(1.f, 0.f, 0.f),
};

Piece::Piece(PieceName type)
{
    blocks = new Block*[4];
    for(int i=0;i<4;i++) blocks[i] = new Block(16, piece_colours[static_cast<int>(type)][0], \
        piece_colours[static_cast<int>(type)][1], piece_colours[static_cast<int>(type)][2]);
    piece_type = type;
}

std::vector<glm::vec2> Piece::getPos()
{
    return block_pos;
}

void Piece::setOrigin()
{
    block_pos.clear();
    block_pos.push_back( glm::vec2(0,0) );
    block_pos.push_back( glm::vec2(1,0) );

    if(piece_type == PIECE_I or piece_type == PIECE_J or piece_type == PIECE_L or piece_type == PIECE_T)
    {
        block_pos.push_back( glm::vec2(2,0) );
        if(piece_type == PIECE_I) block_pos.push_back( glm::vec2(3,0) );
        else if(piece_type == PIECE_J) block_pos.push_back( glm::vec2(2,1) );
        else if(piece_type == PIECE_L) {
            block_pos.pop_back();
            block_pos.push_back( glm::vec2(1,1) );
            block_pos.push_back( glm::vec2(1,2) );
            }
        else if(piece_type == PIECE_T) {
            block_pos.pop_back();
            block_pos.pop_back();
            block_pos.push_back( glm::vec2(1,1) );
            block_pos.push_back( glm::vec2(1,0) );
            block_pos.push_back( glm::vec2(1*2,0) );
        }
        else fprintf(stderr, "invalid piece1\n");
    }
    else if (piece_type == PIECE_Z or piece_type == PIECE_O)
    {
        block_pos.push_back( glm::vec2(1,1) );
        if(piece_type == PIECE_Z) block_pos.push_back( glm::vec2(2,1) );
        else if(piece_type == PIECE_O) block_pos.push_back( glm::vec2(0,1) );
        else fprintf(stderr, "invalid piece %d\n", static_cast<int>(piece_type));
    }
    else if (piece_type == PIECE_S)
    {
        block_pos.pop_back();
        block_pos.pop_back();
        block_pos.push_back( glm::vec2(0,1) );
        block_pos.push_back( glm::vec2(1,1) );
        block_pos.push_back( glm::vec2(1,0) );
        block_pos.push_back( glm::vec2(2,0) );
    }
    else fprintf(stderr, "invalid piece3\n");
}

void Piece::setActive()
{
    setOrigin();
    move(4.f,0.f);
}

void Piece::setNext()
{
    setOrigin();
    move(13.f, 6.f);
}

void Piece::setHold()
{
    setOrigin();
    move(13.f, 14.f);
}

void Piece::move(float x, float y)
{
    for (int i = 0; i < 4; i++)
    {
        block_pos[i][0] = block_pos[i][0] + x;
        block_pos[i][1] = block_pos[i][1] + y;
    }

}

void Piece::draw(glm::mat4* projection, GLuint MVPID, GLuint colourID)
{
    for(int i=0;i<4;i++)
        blocks[i]->draw(projection,MVPID,colourID,block_pos[i]);
}

void Piece::drop()
{
    move(0.f, 1.f);
}

glm::vec3 Piece::getColour()
{
    return piece_colours[static_cast<int>(piece_type)];
}

void Piece::shift(bool right)
{
    bool movable = true;

    for(int i=0;i<4;i++)
    {
        if(right && block_pos[i][0]>=9) movable = false;
        else if(!right && block_pos[i][0]<=0) movable = false;
    }

    if(movable)
    {
        if(right) move(1.f,0);
        else move(-1.f,0);
        for(int i=0;i<4;i++)
        {
            if(right) blocks[i]->move(1.f,0);
            else blocks[i]->move(-1.f,0);
        }
    }
}

void Piece::rotate(bool cw)
{
    if( piece_type != PIECE_O)
    {
        bool in_bounds = true;
        std::vector<glm::vec2> new_pos;
        glm::vec2 new_dir, old_pos, middle_block, dir_from_mid;
        middle_block = block_pos[2];

        for (int i = 0; i < 4; ++i)
        {
            old_pos = block_pos[i];
            new_dir = glm::vec2();
            dir_from_mid[0] = middle_block[0] - old_pos[0];
            dir_from_mid[1] = middle_block[1] - old_pos[1];
            if(cw)
            {
                new_dir[0] = dir_from_mid[1];
                new_dir[1] = 0-dir_from_mid[0];
            }
            else
            {
                new_dir[0] = dir_from_mid[1];
                new_dir[1] = 0-dir_from_mid[0];
            }
            new_pos.push_back( glm::vec2( (new_dir[0] + middle_block[0]), (new_dir[1] + middle_block[1]) ) );
        }

        for (int i = 0; i < 4; ++i)
        {
            if(new_pos[i][0] < 0 || new_pos[i][0] > 9) in_bounds = false;
        }

        if(in_bounds)
        {
            block_pos.clear();
            for (int i = 0; i < 4; ++i) block_pos.push_back(new_pos[i]);
        }
    }
}
