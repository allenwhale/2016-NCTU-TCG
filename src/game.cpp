#include "game.h"

Game::Game(): move(defaultAi), evil(defaultAi){}

Game::Game(reference_wrapper<Ai> _move, reference_wrapper<Ai> _evil): move(_move), evil(_evil) {}

void Game::setMove(reference_wrapper<Ai> _move){
    move = _move;
}

void Game::setEvil(reference_wrapper<Ai> _evil){
    evil = _evil;
}

unsigned Game::generateMove(){
    unsigned score;
    score = board.move(move.get().generateMove(board));
    return score;
}

void Game::generateEvil(){
    int index = evil.get().generateEvil(board);
    board.set(index / 4, index % 4, board.getNextTile());
}

void Game::gameOver(){
    move.get().gameOver(board, AI_ROLE_MOVE);
    evil.get().gameOver(board, AI_ROLE_EVIL);
}

void Game::run(const string &logFile, int games){
    Statistics statistics;
    for(int i=0;i<games;i++){
        unsigned score = 0, round = 0;
        board = Board();
        generateEvil();
        generateEvil();
        while(true){
            if(board.isEnd())break;
            score += statistics.runMove(bind(&Game::generateMove, this));
            statistics.runEvil(bind(&Game::generateEvil, this));
            round++;
        }
        gameOver();
        statistics.update(score, round, Helper::getFib(board.getMaxTile()));
        Helper::clearScreen();
        statistics.show();
        if(statistics.round == 1000){
            statistics.dump(logFile);
            statistics.reset();
        }
    }
    statistics.dump(logFile);
}
