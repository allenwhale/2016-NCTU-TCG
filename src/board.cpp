#include "board.h"

Board::Board(): d(0) {}

Board::Board(__int128 x): d(x) {}

Board::Board(const Board &n): d(n.d) {}

Board::Board(unsigned board[4][4]): d(0) {
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            for(int k=0;;k++){
                if(Helper::getFib(k) == board[i][j]){
                    this->set(i, j, k);
                    break;
                }
            }
        }
    }
}

Board& Board::operator = (const Board &n) {
    d = n.d;
    return *this;
}

bool Board::operator == (const Board &n) const {
    return d == n.d;
}

bool Board::operator != (const Board &n) const {
    return !((*this) == n);
}

unsigned Board::get(int x, int y) const {
    return (d >> ((x * 4 + y) * 5)) & 0x1f;
}

unsigned Board::getRow(int x) const {
    return (d >> (x * 4 * 5)) & 0xfffff;
}

unsigned Board::getCol(int x) const {
    __int128 nd = d >> (x * 5);
    return (nd & 0x1f) | ((nd >> 15) & 0x3e0) | ((nd >> 30) & 0x7c00) | ((nd >> 45) & 0xf8000);
}

void Board::set(int x, int y, unsigned v) {
    d |= __int128(v) << ((x * 4 + y) * 5);
}

void Board::setRow(int x, unsigned v) {
    d |= __int128(v) << (x * 4 * 5);
}

void Board::setCol(int x, unsigned v) {
    __int128 nv = v;
    d |= ((nv & 0x1f) | ((nv & 0x3e0) << 15) | ((nv & 0x7c00) << 30) | ((nv & 0xf8000) << 45)) << (x * 5);
}

unsigned Board::getMaxTile() const {
    unsigned res = 0;
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            res = max(res, get(i, j));
        }
    }
    return res;
}

unsigned Board::move(int dir){
    //URDL
    Board nb;
    unsigned score = 0;
    if(dir == 0 || dir == 2){
        for(int i=0;i<4;i++){
            unsigned col = getCol(i);
            nb.setCol(i, Table::moveTable[dir == 2][col]);
            score += Table::moveScore[dir == 2][col];
        }
    }else if(dir == 1 || dir == 3){
        for(int i=0;i<4;i++){
            unsigned row = getRow(i);
            nb.setRow(i, Table::moveTable[dir == 1][row]);
            score += Table::moveScore[dir == 1][row];
        }
    }
    d = nb.d;
    return score;
}

void Board::rotateRight(){
    Board nb;
    for(int i=0;i<4;i++){
        nb.setCol(3 - i, getRow(i));
    }
    d = nb.d;
}

void Board::reflectUpDown(){
    Board nb;
    for(int i=0;i<4;i++){
        nb.setRow(3 - i, getRow(i));
    }
    d = nb.d;
}

void Board::print() const {
    puts("---");
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            printf("%5d", Helper::getFib(get(i, j)));
        }
        printf("\n");
    }
}

bool Board::isEnd() const {
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            if(get(i, j) == 0){
                return false;
            }
        }
    }
    for(int i=0;i<4;i++){
        Board nb = *this;
        nb.move(i);
        if(nb != *this)
            return false;
    }
    return true;
}
