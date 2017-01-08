#include "expecti.h"

pair<double, int> Expecti::max(const Board &b, int dep, const function<double(const Board&)> &evaluate){
    pair<double, int> res = {0, 0};
    if(dep == 0 || b.isEnd()){ //end
        res.first = evaluate(b);
    }else if(dep & 1){ //move
        res.first = -1e15;
        double value[4] = {-1e15, -1e15, -1e15, -1e15};
#pragma omp parallel for num_threads(THREAD_NUM)
        for(int i=0;i<4;i++){
            Board nb = b;
            double tmpValue = nb.move(i);
            if(nb == b)continue;
            tmpValue += max(nb, dep - 1, evaluate).first;
            value[i] = tmpValue;
        }
        for(int i=0;i<4;i++){
            if(Helper::cmpDouble(value[i], res.first) > 0){
                res.second = i;
                res.first = value[i];
            }
        }
    }else{ //evil
        int setTile = b.getNextTile();
        int cnt = res.first = 0;
        for(int i=0;i<4;i++){
            for(int j=0;j<4;j++){
                if(b.get(i, j) == 0){
                    Board nb = b;
                    nb.set(i, j, setTile);
                    double value = max(nb, dep - 1, evaluate).first;
                    res.first += value;
                    cnt++;
                }
            }
        }
        res.first /= (double)cnt;
    }
    return res;
}

pair<double, int> Expecti::min(const Board &b, int dep, const function<double(const Board&)> &evaluate){
    pair<double, int> res = {0, 0};
    if(dep == 0 || b.isEnd()){ //end
        res.first = evaluate(b);
    }else if(dep & 1){ //move
        res.first = 0;
        double value[4] = {-1e15, -1e15, -1e15, -1e15};
#pragma omp parallel for num_threads(THREAD_NUM)
        for(int i=0;i<4;i++){
            Board nb = b;
            double tmpValue = nb.move(i);
            if(nb == b)continue;
            tmpValue += max(nb, dep - 1, evaluate).first;
            value[i] = tmpValue;
        }
        int cnt = 0;
        for(int i=0;i<4;i++){
            if(Helper::cmpDouble(value[i], -1e15)){
                cnt++;
                res.first += value[i];
            }
        }
        res.first /= (double)cnt;
    }else{ //evil
        int setTile = b.getNextTile();
        res.first = 1e15;
        for(int i=0;i<4;i++){
            for(int j=0;j<4;j++){
                if(b.get(i, j) == 0){
                    Board nb = b;
                    nb.set(i, j, setTile);
                    double value = min(nb, dep - 1, evaluate).first;
                    if(Helper::cmpDouble(res.first, value) > 0){
                        res.first = value;
                        res.second = i * 4 + j;
                    }
                }
            }
        }
    }
    return res;
}
