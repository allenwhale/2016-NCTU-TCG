#include "table.h"
unsigned Table::moveTable[2][1 << 20];
unsigned Table::moveScore[2][1 << 20];

void Table::initialize(){
    for(int i=0;i<(1<<20);i++){
        auto res = Table::move(i);
        Table::moveTable[0][i] = res.first;
        Table::moveScore[0][i] = res.second;
        res = Table::move(Table::reverse(i));
        Table::moveTable[1][i] = Table::reverse(res.first);
        Table::moveScore[1][i] = res.second;
    }
}

unsigned Table::reverse(unsigned v){
    unsigned res = 0;
    for(int i=0;i<4;i++){
        res = (res << 5) | (v & 0x1f);
        v >>= 5;
    }
    return res;
}
pair<unsigned, unsigned> Table::move(unsigned row){
    static unsigned mask[] = {0x1f, 0x1f << 5, 0x1f << 10, 0x1f << 15};
    unsigned score = 0;
    bool merged[4] = {0};
    for(int i=1;i<4;i++){
        if(row & mask[i]){
            unsigned nowTile = row & mask[i];
            row &= ~nowTile;
            int j = i - 1;
            while(j >= 0 && !(row & mask[j])){
                j--;
                nowTile >>= 5;
            }
            row |= nowTile;
            unsigned nextTile = row & mask[j];
            if(j >= 0 && nextTile){
                unsigned now = (nowTile >> ((j + 1) * 5)) & 0x1f;
                unsigned nxt = (nextTile >> (j * 5)) & 0x1f;
                if(!merged[j] && Table::canMerge(now, nxt)){
                    row &= ~nowTile;
                    row &= ~nextTile;
                    nxt = max(nxt, now) + 1;
                    score += Helper::getFib(nxt); 
                    row |= nxt << (j * 5);
                    merged[j] = true;
                }
            } 
        }
    }
    return {row, score};
}

bool Table::canMerge(int a, int b){
    return a && b && ((a == 1 && b == 1) || (abs(a - b) == 1));
}
