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
        vector<double>* leftQAS; 
        vector<double>* leftQBS; 
        vector<double>* leftQCS; 
        vector<double>* rightQS; 
        vector<double>* rightQAS; 
        vector<double>* rightQBS; 
        vector<double>* rightQCS; 
        double leftVisits;
        double rightVisits;

        LeafSplit(int, double, vector<double>*, vector<double>*, vector<double>*, 
                        vector<double>*, vector<double>*, vector<double>*, 
                        vector<double>*, vector<double>*, double, double);
        void update(State*, Action*, int, unordered_map<string, double>*);
        void updateA(State*, Action*, int, unordered_map<string, double>*);
        void updateB(State*, Action*, int, unordered_map<string, double>*);
        void updateC(State*, Action*, int, unordered_map<string, double>*);
        double evalUtility(vector<double>*);
        double evalUtilityA(vector<double>*);
        double evalUtilityB(vector<double>*);
        double evalUtilityC(vector<double>*);
};
#endif
