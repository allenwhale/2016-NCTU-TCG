#ifndef _TDLAMBDA_H_
#define _TDLAMBDA_H_

#include <bits/stdc++.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include "json.hpp"
#include "board.h"
#include "helper.h"
#include "ai.h"
#include "ab.h"
#include "expectimax.h"
#include "td.h"


class TDLambda : public TD{
    public:
        TDLambda();
        TDLambda(const string&);
        ~TDLambda();
        void adjust(const Board&);
};

#endif
