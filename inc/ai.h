#ifndef _AI_H_
#define _AI_H_

#include <bits/stdc++.h>
#include <helper.h>
#include "board.h"

#define AI_ROLE_MOVE 0 
#define AI_ROLE_EVIL 1

class Ai{
    public:
        virtual int generateMove(const Board&);
        virtual int generateEvil(const Board&);
        virtual void gameOver(const Board&, int);
};
#endif
