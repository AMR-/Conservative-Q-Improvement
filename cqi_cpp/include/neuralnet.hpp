#include <vector>

class NeuralNet {
    public:
        double learning_rate;
        vector<double> X;
        vector<double> y;
        vector<double> W;

        NeuralNet(double); // weights are randomly initialized here
        vector<double> sigmoid();
        vector<double> sigmoid_d(); // derivative of sigmoid
};
