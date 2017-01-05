#include "expectimax.h"

pair<double, int> ExpectiMax::search(const Board &b, int dep, const function<double(const Board&)> &evaluate){
    pair<double, int> res = {0, 0};
    if(dep == 0 || b.isEnd()){ //end
        res.first = evaluate(b);
    }else if(dep & 1){ //move
        res.first = -1e15;
#pragma omp parallel for num_threads(THREAD_NUM)
        for(int i=0;i<4;i++){
            Board nb = b;
            double value = nb.move(i);
            if(nb == b)continue;
            value += search(nb, dep - 1, evaluate).first;
            if(Helper::cmpDouble(value, res.first) > 0){
                res.second = i;
                res.first = value;
            }
        }
    }else{ //evil
        int cnt = res.first = 0;
        for(int i=0;i<4;i++){
            for(int j=0;j<4;j++){
                if(b.get(i, j) == 0){
                    Board nb = b;
                    nb.set(i, j, 1);
                    double value = search(nb, dep - 1, evaluate).first;
                    res.first += value;
                    cnt++;
                }
            }
        }
        res.first /= (double)cnt;
    }
    return res;
}
