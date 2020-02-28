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
        vector<double>* rightQS; 
        double leftVisits;
        double rightVisits;

        LeafSplit(int, double, vector<double>*, vector<double>*, double, double);
        ~LeafSplit();

        void update(State*, Action*, int, unordered_map<string, double>*);
        double evalUtility(vector<double>*);
};
#endif
