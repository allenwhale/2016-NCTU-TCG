#ifndef _GAME_H_
#define _GAME_H_

#include <bits/stdc++.h>
#include "ai.h"
#include "board.h"
#include "statistics.h"
#include "helper.h"

using namespace std;

static Ai defaultAi;

class Game{
    public:
        reference_wrapper<Ai> move, evil;
        Board board;
        Game();
        Game(reference_wrapper<Ai>, reference_wrapper<Ai>);
        void setMove(reference_wrapper<Ai>);
        void setEvil(reference_wrapper<Ai>);
        unsigned generateMove();
        void generateEvil();
        int getNextTile();
        void gameOver();
        void run(const string&, int);
};
#endif
