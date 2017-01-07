#ifndef _TD_H_
#define _TD_H_

#include <bits/stdc++.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include "json.hpp"
#include "board.h"
#include "helper.h"
#include "ai.h"
#include "ab.h"
#include "expectimax.h"

using namespace std;
using namespace std::placeholders;
using json = nlohmann::json;

#define TD_BASE (23)

class TDFeature;
class TDFeatureTemplate;
class TDConfig;
class TD;

class TDFeature{
    public:
        unsigned feature;
        TDFeatureTemplate *featureTemplate;
        TDFeature();
        TDFeature(unsigned, TDFeatureTemplate*);
        double getValue() const;
        double getError() const;
        double getAbsError() const;
        void adjust(double);
        void adjustError(double);
        void adjustAbsError(double);
};

class TDFeatureList{
    public:
        vector<TDFeature> featureList;
        unsigned score;
        double value;
        void append(const TDFeature&);
        void extend(const TDFeatureList&);
        double getValue() const;
        void adjust(double);
};

class TDFeatureTemplate{
    public:
        string name;
        int type;
        vector<int> indexes;
        vector<double> weightTable;
        vector<double> errorTable;
        vector<double> absErrorTable;
        TDFeatureTemplate();
        ~TDFeatureTemplate();
        TDFeatureTemplate(const string&, int, const vector<int>&);
        unsigned getFeature(const Board&) const;
        double evaluate(const Board&) const;
        void save();
        TDFeatureList operator () (const Board&);
};

class TDConfig{
    public:
        string name;
        bool train;
        double learningRate, lambda;
        vector<TDFeatureTemplate> featureTemplateList;
        TDConfig();
        TDConfig(const string&);
        void loadConfig(const string&);
};

class TD : public Ai{
    public:
        TDConfig config;
        vector<TDFeatureList> featureStack;
        AlphaBeta ab;
        ExpectiMax expectiMax;
        TD();
        ~TD();
        TD(const string&);
        void loadConfig(const string&);
        double evaluate(const Board&);
        void adjust(const Board&);
        void save();
        virtual int generateMove(const Board&);
        virtual int generateEvil(const Board&);
        virtual void gameOver(const Board&, int);
        TDFeatureList operator () (const Board&, unsigned);
};

#endif
