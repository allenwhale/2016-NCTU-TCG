#include "client.h"

Client::Client(): Game() {}

Client::Client(const string &_host, int _port, reference_wrapper<Ai> _move, reference_wrapper<Ai> _evil): Game(_move, _evil), host(_host), port(_port) {}

void Client::setHost(const string &_host){
    host = _host;
}

void Client::setPort(int _port){
    port = _port;
}

void Client::setStudentId(const string &_studentId){
    studentId = _studentId;
}

void Client::setPasswd(const string &_passwd){
    passwd = _passwd;
}

void Client::connect(){
    sockFd = socket(PF_INET, SOCK_STREAM, 0);
    if(sockFd == -1){
        perror("socker: ");
        exit(1);
    }
    struct sockaddr_in dest;
    bzero(&dest, sizeof(dest));
    dest.sin_family = PF_INET;
    dest.sin_port = htons(port);
    dest.sin_addr.s_addr = inet_addr(host.c_str());
    if(::connect(sockFd, (struct sockaddr*)&dest, sizeof(dest)) == -1){
        perror("connect: ");
        exit(1);
    }
}

void Client::send(const string &msg){
    assert(write(sockFd, msg.c_str(), msg.size()) == (int)msg.size());
    if(msg.back() != '\n'){
        assert(write(sockFd, "\n", 1) == 1);
    }
}

void Client::run(){
    unsigned rawBoard[4][4];
    int oldStdin = dup(0);
    dup2(sockFd, 0);
    send("AI " + studentId + "\n");
    send("init " + passwd + "\n\n");
    while(1){
        printf("Waiting Msg\n");
        string msg;
        getline(cin, msg);
        if(msg == ""){
            printf("Server Closed\n");
            break;
        }
        printf("Receive Msg: %s\n", msg.c_str());
        if(msg.find("genmove") != string::npos){
            size_t playStringPos = msg.find(" play");
            size_t evilStringPos = msg.find(" evil");
            size_t jobIdStringPos = msg.find(" jid:");
            int role = -1;
            int jobId = -1;
            string boardString = "";
            if(playStringPos != string::npos){
                boardString = msg.substr(playStringPos + 5, jobIdStringPos - (playStringPos + 5));
                role = AI_ROLE_MOVE;
            }else if(evilStringPos != string::npos){
                boardString = msg.substr(evilStringPos + 5, jobIdStringPos - (evilStringPos + 5));
                role = AI_ROLE_EVIL;
            }else{
                continue;
            }
            if(jobIdStringPos != string::npos){
                jobId = stoi(msg.substr(jobIdStringPos + 5));
            }
            stringstream ss(boardString);
            int idx = 0, tile;
            while(ss >> tile){
                rawBoard[idx >> 2][idx & 3] = tile;
                idx++;
            }
            Board board(rawBoard);
            string res = "";
            if(role == AI_ROLE_MOVE){
                res = to_string(move.get().generateMove(board));
            }else if(role == AI_ROLE_EVIL){
                res = to_string(evil.get().generateEvil(board));
            }
            send("AI " +studentId + "\n");
            send(res + " jid:" + to_string(jobId) + "\n\n");
            printf("Send Job %d response: AI %s\n%s\n", jobId, studentId.c_str(), res.c_str());
        }else if(msg.find("gameover") != string::npos){
        }
    }
    
    dup2(oldStdin, 0);
    close(sockFd);
}
