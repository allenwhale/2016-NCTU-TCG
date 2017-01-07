#ifndef _EXPECTI_H_
#define _EXPECTI_H_

#include <bits/stdc++.h>
#include "board.h"
#include "helper.h"
#include <omp.h>
using namespace std;

class Expecti{
    public:
        pair<double, int> max(const Board&, int, const function<double(const Board&)>&);
};

#endif
