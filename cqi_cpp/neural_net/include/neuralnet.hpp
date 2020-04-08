#include <vector>
#include <math.h>
#include <iostream>
#include <random>

using namespace std;

class NeuralNet {
    public:
        double learning_rate;
        /* vector<double> X; */
        /* vector<double> y; */
        /* vector<double> W; */
        int layers;
        int nodes_per_layer;

        NeuralNet(double, int, int); // weights are randomly initialized here
        vector<double>* dot(vector<double>*, vector<double>*, int, int, int);
        vector<double>* sigmoid(vector<double>*);
        vector<double>* sigmoid_d(vector<double>*); // derivative of sigmoid
        vector<double>* transpose(vector<double>*, int, int); 
        void print_matrix(vector<double>*, int, int);
        vector<double>* sub(vector<double>*, vector<double>*);
        vector<double>* add(vector<double>*, vector<double>*);
        vector<double>* elem_mult(vector<double>*, vector<double>*);
        vector<double>* random_matrix(int, int);  
};
