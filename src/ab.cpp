#include "ab.h"

pair<double, int> AlphaBeta::search(const Board &b, double alpha, double beta, int dep, const function<double(const Board&)> &evaluate){
    pair<double, int> res = {0, 0};
    if(dep == 0 || b.isEnd()){ //end
        res.first = evaluate(b);
    }else if(dep & 1){ //move
        double maxValue = -1e15;
        for(int i=0;i<4;i++){
            Board nb = b;
            double value = nb.move(i);
            if(nb == b)continue;
            value += search(nb, alpha, beta, dep - 1, evaluate).first;
            if(Helper::cmpDouble(value, maxValue) > 0){
                res.second = i;
                maxValue = value;
            }
            alpha = max(alpha, maxValue);
            if(Helper::cmpDouble(alpha, beta) >= 0){
                break;
            }
        }
        res.first = max(maxValue, alpha);
    }else{ //evil
        double minValue = 1e15;
        for(int i=0;i<4;i++){
            for(int j=0;j<4;j++){
                if(b.get(i, j) == 0){
                    Board nb = b;
                    nb.set(i, j, 1);
                    double value = search(nb, alpha, beta, dep - 1, evaluate).first;
                    if(Helper::cmpDouble(value, minValue) < 0){
                        minValue = value;
                        res.second = (i << 2) | j;
                    }
                    beta = min(beta, minValue);
                    if(Helper::cmpDouble(alpha, beta) >= 0){
                        break;
                    }
                }
            }
        }
        res.first = min(minValue, beta);
    }
    return res;
}
