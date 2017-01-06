#include "td.h"


TDFeature::TDFeature(){}

TDFeature::TDFeature(unsigned _feature, TDFeatureTemplate *_featureTemplate): feature(_feature), featureTemplate(_featureTemplate) {}

double TDFeature::getValue() const {
    return featureTemplate->weightTable[feature];
}

void TDFeature::adjust(double delta){
    featureTemplate->weightTable[feature] += delta;
}

void TDFeatureList::append(const TDFeature &feature){
    featureList.push_back(feature);
}

void TDFeatureList::extend(const TDFeatureList &nxtFeatureList){
    featureList.insert(featureList.end(), nxtFeatureList.featureList.begin(), nxtFeatureList.featureList.end());
}

double TDFeatureList::getValue() const {
    double res = 0;
    for(auto &feature : featureList){
        res += feature.getValue();
    }
    return res;
}

void TDFeatureList::adjust(double delta){
    for(auto &feature : featureList){
        auto featureTemplate = feature.featureTemplate;
        double alpha = 1;
        if(Helper::cmpDouble(0, featureTemplate->absErrorTable[feature.feature]) != 0){
            alpha = abs(featureTemplate->errorTable[feature.feature]) / featureTemplate->absErrorTable[feature.feature];
        }
        feature.adjust(delta * alpha);
        featureTemplate->absErrorTable[feature.feature] += abs(delta);
        featureTemplate->errorTable[feature.feature] += delta;
    }
}

TDFeatureTemplate::TDFeatureTemplate(){}

TDFeatureTemplate::TDFeatureTemplate(const string &_name, int _type, const vector<int> &_indexes): name(_name), type(_type), indexes(_indexes) {
    int size = Helper::pow(22, indexes.size());
    static string tableTypes[] = {"weight", "error", "abs_error"};
    reference_wrapper<vector<double>> tables[] = {weightTable, errorTable, absErrorTable};
#pragma omp parallel for num_threads(THREAD_NUM)
    for(int i=0;i<3;i++){
        tables[i].get().resize(size);
        int fd = open((name + "_" + tableTypes[i] + ".dat." + to_string(type)).c_str(), O_RDONLY);
        if(fd == -1){
            continue;
        }
        int readSize = sizeof(tables[i].get()[0]) * size;
        if(read(fd, &(tables[i].get()[0]), readSize) != readSize){
            continue;
        }
    }
}

TDFeatureTemplate::~TDFeatureTemplate(){
    save();
}

void TDFeatureTemplate::save(){
    int size = Helper::pow(22, indexes.size());
    static string tableTypes[] = {"weight", "error", "abs_error"};
    reference_wrapper<vector<double>> tables[] = {weightTable, errorTable, absErrorTable};
#pragma omp parallel for num_threads(THREAD_NUM)
    for(int i=0;i<3;i++){
        int fd = open((name + "_" + tableTypes[i] + ".dat." + to_string(type)).c_str(), O_WRONLY | O_CREAT, 0644);
        if(fd == -1){
            continue;
        }
        fsync(fd);
        int writeSize = sizeof(tables[i].get()[0]) * size;
        if(write(fd, &(tables[i].get()[0]), writeSize) != writeSize){
            continue;
        }
    }
}

TDFeatureList TDFeatureTemplate::operator () (const Board &b){
    TDFeatureList res;
    Board nb = b;
    for(int i=0;i<2;i++){
        for(int j=0;j<4;j++){
            res.append(TDFeature(getFeature(nb), this));
            nb.rotateRight();
        }
        nb.reflectUpDown();
    }
    return res;
}

TDConfig::TDConfig(){}

TDConfig::TDConfig(const string &configFile){
    loadConfig(configFile);
}

unsigned TDFeatureTemplate::getFeature(const Board &b) const {
    unsigned feature = 0;
    for(int idx : indexes){
        feature = feature * 22 + b.get(idx >> 2, idx & 3);
    }
    return feature;
}

double TDFeatureTemplate::evaluate(const Board &b) const {
    return weightTable[getFeature(b)];
}

void TDConfig::loadConfig(const string &configFile){
    ifstream i(configFile.c_str());
    json j;
    i >> j;
    name = j["name"];
    learningRate = j["learningRate"];
    train = j["train"];
    if(j.find("lambda") != j.end()){
        lambda = j["lambda"];
    }
    int featureType = 0;
    featureTemplateList.clear();
    for(auto &indexes : j["features"]){
        featureTemplateList.push_back(TDFeatureTemplate(name, featureType, indexes));
        featureType++;
    }
}

TD::TD(){}

TD::TD(const string &configFile): config(configFile) {}

TD::~TD(){
    save();
}

void TD::loadConfig(const string &configFile){
    config.loadConfig(configFile);
}

double TD::evaluate(const Board &b){
    return (*this)(b, 0).getValue();
}

void TD::adjust(const Board &b){
    if(config.train){
        auto &featureStack = this->featureStack;
        double delta;
        auto featureList = (*this)(b, 0);
        delta = -featureList.getValue();
        featureList.adjust(delta * config.learningRate / (double)config.featureTemplateList.size() / 8.);
        auto nxtFeatureList = featureList;
        while(featureStack.size()){
            featureList = featureStack.back();
            featureStack.pop_back();
            delta = nxtFeatureList.getValue() + (double)nxtFeatureList.score - featureList.getValue();
            featureList.adjust(delta * config.learningRate / (double)config.featureTemplateList.size() / 8.);
            nxtFeatureList = featureList;
        }
    }
}

void TD::save(){
    if(config.train == false)return;
    for(auto &featureTemplate : config.featureTemplateList){
        featureTemplate.save();
    }
}

int TD::generateMove(const Board &b){
    auto res = expectiMax.search(b, 3, bind(&TD::evaluate, this, _1));
    if(config.train){
        Board nb = b;
        unsigned score = nb.move(res.second);
        featureStack.push_back((*this)(nb, score));
    }
    return res.second;
}

int TD::generateEvil(const Board &b){
    if(config.train){
        if(rand() % 10 < 3){
            return Ai().generateEvil(b);
        }
    }
    auto res = ab.search(b, -1e15, DBL_MAX, 2, bind(&TD::evaluate, this, _1));
    return res.second;
}

void TD::gameOver(const Board &b, int role){
    static int round = 0;
    if(role == AI_ROLE_MOVE){
        adjust(b);
        if(round == 100){
            save();
            round = 0;
        }
        round ++;
    }
    //getchar();
}

TDFeatureList TD::operator () (const Board &b, unsigned score){
    TDFeatureList res;
    for(auto &featureTemplate : config.featureTemplateList){
        res.extend(featureTemplate(b));
    }
    res.score = score;
    return res;
}
