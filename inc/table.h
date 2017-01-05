#ifndef _TABLE_H_
#define _TABLE_H_

#include <bits/stdc++.h>
#include "helper.h"
using namespace std;

class Table{
    public:
        static unsigned moveTable[2][1 << 20];
        static unsigned moveScore[2][1 << 20];
        static void initialize();
        static unsigned reverse(unsigned);
        static pair<unsigned, unsigned> move(unsigned);
        static bool canMerge(int, int);
};

#endif
