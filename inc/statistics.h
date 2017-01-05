#ifndef _STATISTICS_H_
#define _STATISTICS_H_

#include <bits/stdc++.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include "helper.h"
using namespace std;


class Statistics{
    public:
        double avgScore;
        unsigned maxScore;
        unsigned maxTile, untilMaxTile, untilMaxScore;
        __int128 round, totalRound, winRound610, winRound2584, totalStep, totalMoveInterval, totalEvilInterval;
        vector<int> scoreRangeCount;
        Statistics();
        void reset();
        void update(unsigned, unsigned, unsigned);
        void runEvil(const function<void()>&);
        unsigned runMove(const function<unsigned()>&);
        void show();
        void dump(const string&);
};

#endif
