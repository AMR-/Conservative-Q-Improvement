#ifndef LEAFSPLIT_H
#define LEAFSPLIT_H
#include "state.hpp"
#include "action.hpp"
#include "utils.hpp"
#include <unordered_map>
#include <algorithm>

class LeafSplit {
    public:
        int feature;
        double value;
        vector<double>* leftQS; 
        vector<vector<double>*>* leftQVS;
        vector<double>* rightQS; 
        vector<vector<double>*>* rightQVS;
        double leftVisits;
        double rightVisits;

        LeafSplit(int, double, vector<double>*, vector<double>*, vector<vector<double>*>*, 
                        vector<vector<double>*>*, double, double);
        void update(State*, Action*, int, unordered_map<string, double>*);
        void update(State*, Action*, int, unordered_map<string, double>*, int index);
        double evalUtility(vector<double>*);
        double evalUtility(vector<double>*, int index);
};
#endif
