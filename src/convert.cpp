#include <bits/stdc++.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

int Pow(int x, int n){
    int res = 1;
    for(int i=n;i--;)
        res *= x;
    return res;
}

unsigned rehash(int x, int origBase, int destBase){
    unsigned res = 0;
    unsigned base = 1;
    while(x){
        res += base * (x % origBase);
        x /= origBase;
        base *= destBase;
    }
    return res;
}

void convert(const string &origFile, const string &destFile, int origBase, int destBase, int tuple){
    int origSize = Pow(origBase, tuple), destSize = Pow(destBase, tuple);
    double *orig = new double [origSize], *dest = new double [destSize];
    int origFd = open(origFile.c_str(), O_RDONLY), destFd = open(destFile.c_str(), O_WRONLY | O_CREAT, 0644);
    read(origFd, orig, sizeof(double) * origSize);
    close(origFd);
    for(int i=0;i<origSize;i++){
        dest[rehash(i, origBase, destBase)] = orig[i];
    }
    write(destFd, dest, sizeof(double) * destSize);
    close(destFd);
}

int main(int argc, char **argv){
    string configFile = argv[1];
    int origBase = stoi(argv[2]), destBase = stoi(argv[3]);
    ifstream i(configFile.c_str());
    json j;
    i >> j;
    string name = j["name"];
    int featureType = 0;
    for(auto feature : j["features"]){
        int tuple = feature.size();
        static string tableTypes[] = {"weight", "error", "abs_error"};
        for(auto tableType : tableTypes){
            string origFile = name + "_" + tableType + ".dat." + to_string(featureType);
            string destFile = name + "_new_" + tableType + ".dat." + to_string(featureType);
            convert(origFile, destFile, origBase, destBase, tuple);
        }
        featureType++;
    }
    return 0;
}
