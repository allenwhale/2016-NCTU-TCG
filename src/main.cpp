#include <bits/stdc++.h>
#include <unistd.h>
#include "board.h"
#include "table.h"
#include "td.h"
#include "helper.h"
#include "game.h"
#include "client.h"
using namespace std;


void help(){
    printf("Usage: ai [OPTIONS]\n\n");
    printf("\t-t, train\n");
    printf("\t-o, online\n");
    printf("\t-c configFile\n");
    printf("\t-l logFile\n");
    printf("\t-n gameRound\n");
    exit(0);
}

int main(int argc, char **argv){
    srand(time(0));
    Helper::initialize();
    Table::initialize();
    char c;
    string configFile, logFile;
    int gameRound = -1;
    bool trainMode = false, onlineMode = false;
    while((c = getopt(argc, argv, "toc:n:l:h")) != -1){
        switch(c){
            case 't':
                trainMode = true;
                break;
            case 'o':
                onlineMode = true;
                break;
            case 'c':
                configFile = optarg;
                break;
            case 'n':
                gameRound = stoi(optarg);
                break;
            case 'l':
                logFile = optarg;
                break;
            case 'h':
                help();
                break;
        }
    }
    if(configFile == "" || Helper::fileExists(configFile) == false){
        printf("config file [%s] doesn\'t exist\n", configFile.c_str());
        exit(0);
    }
    if(!(trainMode ^ onlineMode)){
        printf("train or online\n");
        exit(0);
    }
    if(trainMode){
        if(gameRound == -1 || logFile == ""){
            help();
        }
    }
    TD td(configFile);
    if(trainMode){
        Game game;
        game.setMove(td);
        //game.setEvil(td);
        game.run(logFile, gameRound);
    }else if(onlineMode){
        Client client;
        client.setHost("140.113.194.120");
        client.setPort(21684);
        client.setStudentId("0316320");
        client.setPasswd("0316320");
        client.setMove(td);
        //client.setEvil(td);
        client.connect();
        client.run();
    }
    return 0;
}
