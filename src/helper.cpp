#include "helper.h"

unsigned Helper::fib[Helper::fibCnt];

void Helper::initialize(){
    fib[0] = 0;
    fib[1] = 1;
    fib[2] = 2;
    for(int i=3;i<Helper::fibCnt;i++){
        fib[i] = fib[i - 1] + fib[i - 2];
    }
}

int Helper::pow(int x, int n){
    int res = 1;
    for(int i=n;i--;)
        res *= x;
    return res;
}

void Helper::clearScreen(){
    printf("\e[2J\e[H");
}

unsigned Helper::getFib(int x){
    return fib[x];
}

int Helper::cmpDouble(double a, double b){
    if(abs(a - b) <= 1e-2){
        return 0;
    }else if(a < b){
        return -1;
    }else{
        return 1;
    }
}

struct timeval Helper::tick(){
    struct timeval res;
    gettimeofday(&res, NULL);
    return res;
}

long long Helper::tock(struct timeval start){
    struct timeval end = tick();
    return (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);
}

bool Helper::fileExists(const string &filename){
    struct stat st;
    return stat(filename.c_str(), &st) == 0;
}
