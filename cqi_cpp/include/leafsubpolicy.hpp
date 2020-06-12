#include "state.hpp"
#include "neuralnet.hpp"

class LeafSubpolicy {
    public:
        NeuralNet* nn;
        State *state;

        LeafSubpolicy();
        void update(); // train using neural network
        selectAction(State*);
};
