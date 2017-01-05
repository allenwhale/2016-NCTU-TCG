#include "tdlambda.h"

TDLambda::TDLambda(){}

TDLambda::TDLambda(const string &configFile): TD(configFile) {}

TDLambda::~TDLambda(){
    if(config.train){
        save();
    }
}

void TDLambda::adjust(const Board &b){
    if(config.train){
        double delta;
        auto featureList = (*this)(b, 0);
        delta = -featureList.getValue();
        featureList.adjust(delta / (double)config.featureTemplateList.size() * config.learningRate);
        featureStack.push_back(featureList);
        double R = 0, nowLambda = 1;
        for(int i=featureStack.size()-2;i>=0;i--,nowLambda*=config.lambda){
            R = config.lambda * R + featureStack[i + 1].getValue();
            double lastR = nowLambda * featureStack.back().getValue();
            double delta = (1. - config.lambda) * (R - lastR) + lastR - featureStack[i].getValue();
            featureStack[i].adjust(delta / (double)config.featureTemplateList.size() * config.learningRate);
        }
    }
}

