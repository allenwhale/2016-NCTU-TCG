#ifndef _AB_H_
#define _AB_H_

#include <bits/stdc++.h>
#include "board.h"
#include "helper.h"
using namespace std;

class AlphaBeta{
    public:
        pair<double, int> search(const Board&, double, double, int, const function<double(const Board&)>&);
};

#endif
