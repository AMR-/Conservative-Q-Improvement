#include "../include/neuronlayer.hpp"

NeuronLayer::NeuronLayer(int inputs_per_node, int nodes) {
    this->inputs_per_node = inputs_per_node;
    this->nodes = nodes;
    this->weights = new vector<double>();

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(-1.0, 1.0);

    for (int i = 0; i < nodes * inputs_per_node; i++) {
       this->weights->push_back(dis(gen));
    }
}
