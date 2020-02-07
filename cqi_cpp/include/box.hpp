#ifndef BOX_H
#define BOX_H
#include "space.hpp"
#include "utils.hpp"
#include <random>

class Box : public Space<vector<float>*> {
    public:
        vector<float>* low;
        vector<float>* high;

        Box(vector<float>*, vector<float>*);
        vector<float>* sample();
        bool contains(vector<float>*);
};
#endif
