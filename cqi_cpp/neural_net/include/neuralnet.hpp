#include <vector>
#include <math.h>
#include <iostream>
#include <algorithm>
#include "../include/neuronlayer.hpp"
#include "../../include/utils.hpp"

using namespace std;

class NeuralNet {
    public:
        double learning_rate;
        int num_layers;
        vector<NeuronLayer*>* layers;

        NeuralNet(double, int); // weights are randomly initialized here
        vector<double>* mat_mult(vector<double>*, vector<double>*, int, int, int);
        vector<double>* loss(vector<double>*);
        vector<double>* loss_d(vector<double>*); // derivative of loss
        vector<double>* transpose(vector<double>*, int, int); 
        void print_matrix(vector<double>*, int, int);
        vector<double>* sub(vector<double>*, vector<double>*);
        vector<double>* add(vector<double>*, vector<double>*);
        vector<double>* elem_mult(vector<double>*, vector<double>*);
        vector<double>* layer(int, int);  
        void train_network(vector<double>*, vector<double>*, int);
        vector<vector<double>*>* think(vector<double>*);
        vector<double>* scalar_mult(int, vector<double>*);
};
