#include "action.hpp"
#include "neuralnet.hpp"

class LeafSubpolicy {
    public:
        Action* action;
        NeuralNet* nn;

        LeafSubpolicy(Action*);
        void update(); // train using neural network
};
