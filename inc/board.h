#ifndef _BOARD_H_
#define _BOARD_H_

#include <stdio.h>
#include "table.h"
#include "helper.h"

class Board{
    public:
        __int128 d;
        Board();
        Board(__int128);
        Board(const Board &);
        Board(unsigned [4][4]);
        Board& operator = (const Board &);
        bool operator == (const Board &) const;
        bool operator != (const Board &) const;
        unsigned get(int, int) const;
        unsigned getRow(int) const;
        unsigned getCol(int) const;
        void set(int, int, unsigned);
        void setRow(int, unsigned);
        void setCol(int, unsigned);
        unsigned getMaxTile() const;
        unsigned getRound() const;
        unsigned getSum() const;
        unsigned move(int);
        void rotateRight();
        void reflectUpDown();
        void print() const;
        bool isEnd() const;
};
#endif
