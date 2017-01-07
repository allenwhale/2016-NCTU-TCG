#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <bits/stdc++.h>
#include <sys/types.h>  
#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "ai.h"
#include "game.h"
#include "board.h"
using namespace std;

static Ai defaultClientAi;

class Client: public Game{
    public:
        string host;
        int port;
        int sockFd;
        string studentId, passwd;
        Client();
        Client(const string&, int, reference_wrapper<Ai>, reference_wrapper<Ai>);
        void setHost(const string&);
        void setPort(int);
        void setStudentId(const string&);
        void setPasswd(const string&);
        void connect();
        void send(const string&);
        void run();
};
#endif
