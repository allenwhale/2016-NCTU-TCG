#include "ai.h"

int Ai::generateMove(const Board &b){
    for(int i=0;i<4;i++){
        Board nb = b;
        nb.move(i);
        if(nb != b){
            return i;
        }
    }
    return rand() % 4;
}

int Ai::generateEvil(const Board &b){
    int cnt = 0;
    for(int i=0;i<16;i++){
        if(!b.get(i >> 2, i & 3)){
            cnt++;
        }
    }
    int idx = rand() % cnt + 1;
    for(int i=0;i<16;i++){
        if(!b.get(i >> 2, i & 3)){
            idx--;
            if(idx == 0){
                return i;   
            }
        }
    }
    return rand() % 16;
}

void Ai::gameOver(const Board &b, int role){
    if(role == AI_ROLE_MOVE){
        b.print();
    }else if(role == AI_ROLE_EVIL){
    }else{
        b.print();
    }
}
