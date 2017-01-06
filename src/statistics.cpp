#include "statistics.h"

Statistics::Statistics(): untilMaxTile(0), untilMaxScore(0), totalRound(0), scoreRangeCount(50) {
    reset();
}

void Statistics::reset(){
    avgScore = 0;
    maxTile = 0;
    maxScore = 0;
    round = 0;
    winRound610 = 0;
    winRound2584 = 0;
    totalStep = 0;
    totalEvilInterval = 0;
    totalMoveInterval = 0;
    fill(scoreRangeCount.begin(), scoreRangeCount.end(), 0);
}

void Statistics::update(unsigned score, unsigned step, unsigned _maxTile){
    scoreRangeCount[score / 10000]++;
    maxScore = max(maxScore, score);
    untilMaxTile = max(untilMaxTile, _maxTile);
    untilMaxScore = max(untilMaxScore, score);
    maxTile = max(maxTile, _maxTile);
    winRound610 += (_maxTile >= 610);
    winRound2584 += (_maxTile >= 2584);
    avgScore *= ((double)round / (double)(round + 1));
    avgScore += (double)score / (double)(round + 1);
    totalStep += step;
    round++;
    totalRound++;
}

void Statistics::runEvil(const function<void()> &func){
    auto start = Helper::tick();
    func();
    long long interval = Helper::tock(start);
    totalEvilInterval += interval;
}

unsigned Statistics::runMove(const function<unsigned()> &func){
    auto start = Helper::tick();
    unsigned res = func();
    long long interval = Helper::tock(start);
    totalMoveInterval += interval;
    return res;
}

void Statistics::show(){
    printf("Total Round         : %lld\n", (long long)totalRound);
    printf("Round               : %lld\n", (long long)round);
    printf("Max Score           : %d\n", maxScore);
    printf("Max Tile            : %d\n", maxTile);
    printf("Until Max Score     : %d\n", untilMaxScore);
    printf("Until Max Tile      : %d\n", untilMaxTile);
    printf("Avg Score           : %f\n", avgScore);
    printf("Win Rate(>=610)     : %f%%\n", 100. * (double)winRound610 / (double)round);
    printf("Win Rate(>=2584)    : %f%%\n", 100. * (double)winRound2584 / (double)round);
    printf("Avg Step            : %f\n", (double)totalStep / (double)round);
    printf("Avg Move Step(us)   : %f\n", (double)totalMoveInterval / (double)totalStep);
    printf("Avg Evil Step(us)   : %f\n", (double)totalEvilInterval / (double)totalStep);
    for(int i=0;i<20;i++){
        printf("%6d ~ %6d: %7.4f%%  %d", 10000 * i, 10000 * (i + 1), 100. * scoreRangeCount[i] / round, scoreRangeCount[i]);
        printf("\t\t");
        printf("%6d ~ %6d: %7.4f%%  %d", 10000 * (i + 20), 10000 * (i + 21), 100. * scoreRangeCount[i + 20] / round, scoreRangeCount[i + 20]);
        puts("");
    }
}

void Statistics::dump(const string &filename){
    int fd = open(filename.c_str(), O_WRONLY | O_APPEND | O_CREAT, 0644);
    int oldStdout = dup(1);
    dup2(fd, 1);
    puts("==================================");
    show();
    dup2(oldStdout, 1);
    close(fd);
}
