#ifndef BOX_H
#define BOX_H
#include "space.hpp"
#include "utils.hpp"
#include <random>

class Box : public Space<vector<double>*> {
    public:
        vector<double>* low;
        vector<double>* high;

        Box(vector<double>*, vector<double>*);
        vector<double>* sample();
        bool contains(vector<double>*);
};
#endif
