#include "action.hpp"
#include "neuralnet.hpp"

class LeafSubpolicy {
    public:
        Action* action;

        LeafSubpolicy(Action*);
        void train(); // train using neural network
};
