#ifndef _HELPER_H_
#define _HELPER_H_

#include <bits/stdc++.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <unistd.h>
using namespace std;

class Helper{
    public:
        const static int fibCnt = 28;
        static unsigned fib[fibCnt];
        static void initialize();
        static int pow(int, int);
        static void clearScreen();
        static unsigned getFib(int);
        static int cmpDouble(double, double);
        static struct timeval tick();
        static long long tock(struct timeval);
        static bool fileExists(const string&);
        static void activePrint();
        static void resetPrint();
};

#endif
