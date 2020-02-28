#ifndef STATE_H
#define STATE_H
#include <vector>
using namespace std;

class State {
    public:
        vector<double>* state; 
        State(vector<double>*);
};
#endif
