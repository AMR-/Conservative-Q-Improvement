#include "../include/neuralnet.hpp"

/* Instructions:
 * This code trains a neural net to predict the labels of four inputs of length four.
 * Run `g++ neuronlayer.cpp neuralnet.cpp test.cpp` and then `./a.out` to see the predicted y values.
 */

int main() {
    int i;

    // Classifying flower data
    vector<double> X {
        5.1, 3.5, 1.4, 0.2,
        4.9, 3.0, 1.4, 0.2, 
        6.2, 3.4, 5.4, 2.3,
        5.9, 3.0, 5.1, 1.8
    };

    vector<double> y {
        0,
        0,
        1,
        1
    };

    NeuralNet *nn = new NeuralNet(1, 1); // learning rate is 1, nnet has 1 layers

    nn->train_network(&X, &y, 30000);
    
    return 0;
}
