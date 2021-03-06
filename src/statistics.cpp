#include "statistics.h"

Statistics::Statistics(): untilMaxTile(0), untilMaxScore(0), totalRound(0), scoreRangeCount(100) {
    reset();
}

void Statistics::reset(){
    avgScore = 0;
    maxTile = 0;
    maxScore = 0;
    round = 0;
    winRound610 = 0;
    winRound2584 = 0;
    winRound10946 = 0;
    totalStep = 0;
    totalEvilInterval = 0;
    totalMoveInterval = 0;
    activeScoreRange = -1;
    fill(scoreRangeCount.begin(), scoreRangeCount.end(), 0);
}

void Statistics::update(unsigned score, unsigned step, unsigned _maxTile){
    scoreRangeCount[activeScoreRange = (score / 20000)]++;
    maxScore = max(maxScore, score);
    untilMaxTile = max(untilMaxTile, _maxTile);
    untilMaxScore = max(untilMaxScore, score);
    maxTile = max(maxTile, _maxTile);
    winRound610 += (_maxTile >= 610);
    winRound2584 += (_maxTile >= 2584);
    winRound10946 += (_maxTile >= 10946);
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
    printf("Round               : %11lld\t\t", (long long)round);
    printf("Total Round         : %11lld\n", (long long)totalRound);

    printf("Max Score           : %11d\t\t", maxScore);
    printf("Until Max Score     : %11d\n", untilMaxScore);

    printf("Max Tile            : %11d\t\t", maxTile);
    printf("Until Max Tile      : %11d\n", untilMaxTile);

    printf("Avg Score           : %11.3f\t\t", avgScore);
    printf("Avg Step            : %11.3f\n", (double)totalStep / (double)round);

    printf("Avg Move Step(us)   : %11.3f\t\t", (double)totalMoveInterval / (double)totalStep);
    printf("Avg Evil Step(us)   : %11.3f\n", (double)totalEvilInterval / (double)totalStep);

    printf("Win Rate(>=610)     : %10.3f%%\t\t", 100. * (double)winRound610 / (double)round);
    printf("Win Rate(>=2584)    : %10.3f%%\n", 100. * (double)winRound2584 / (double)round);
    printf("Win Rate(>=10946)   : %10.3f%%\n", 100. * (double)winRound10946 / (double)round);

    for(int i=0;i<30;i++){
        if(activeScoreRange == i){
            Helper::activePrint();
        }
        printf("%7d ~ %7d: %7.4f%%  %3d", 20000 * i, 20000 * (i + 1), 100. * scoreRangeCount[i] / round, scoreRangeCount[i]);
        Helper::resetPrint();
        printf("\t\t");
        if(activeScoreRange == (i + 30)){
            Helper::activePrint();
        }
        printf("%7d ~ %7d: %7.4f%%  %3d", 20000 * (i + 30), 20000 * (i + 31), 100. * scoreRangeCount[i + 30] / round, scoreRangeCount[i + 30]);
        Helper::resetPrint();
        puts("");
    }
}

void Statistics::dump(const string &filename){
    int fd = open(filename.c_str(), O_WRONLY | O_APPEND | O_CREAT, 0644);
    if(fd == -1){
        perror("dump statistics");
        return;
    }
    int oldStdout = dup(1);
    dup2(fd, 1);
    puts("==================================");
    show();
    dup2(oldStdout, 1);
    fsync(fd);
    close(fd);
}
