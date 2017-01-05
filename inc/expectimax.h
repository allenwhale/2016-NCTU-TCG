#ifndef _EXPECTIMAX_H_
#define _EXPECTIMAX_H_

#include <bits/stdc++.h>
#include "board.h"
#include "helper.h"
#include <omp.h>
using namespace std;

class ExpectiMax{
    public:
        pair<double, int> search(const Board&, int, const function<double(const Board&)>&);
};

#endif
