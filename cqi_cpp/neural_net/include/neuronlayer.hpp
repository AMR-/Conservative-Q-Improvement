#include <vector>
#include <random>

using namespace std;

class NeuronLayer {
    public:
        int nodes;
        int inputs_per_node;
        vector<double>* weights;

        NeuronLayer(int, int); 
};
