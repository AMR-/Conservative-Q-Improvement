#include "../include/neuralnet.hpp"

NeuralNet::NeuralNet(double learning_rate, int num_layers) {
    this->learning_rate = learning_rate;
    this->num_layers = num_layers;

    this->layers = new vector<NeuronLayer*>();

    if (num_layers == 1) {
        this->layers->push_back(new NeuronLayer(4, 1));
    } else {
        // Input layer    
        this->layers->push_back(new NeuronLayer(4, 2));

        // Hidden layers
        for (int i = 0; i < this->num_layers - 2; i++) {
            this->layers->push_back(new NeuronLayer(2, 2));
        }

        // Output layer
        this->layers->push_back(new NeuronLayer(2, 1));
    }
}

vector<double>* 
NeuralNet::mat_mult(vector<double>* m1, vector<double>* m2, int m1_rows, int m1_cols, int m2_cols) {
    vector<double>* res = new vector<double>();
    int row, col, k;

    for (row = 0; row < m1_rows; row++) {
        for (col = 0; col < m2_cols; col++) {
            res->push_back(0.0);
        }
    }

    for (row = 0; row < m1_rows; row++) {
        for (col = 0; col < m2_cols; col++) {
            for (k = 0; k < m1_cols; k++) {
                res->at(row * m2_cols + col) += 
                        m1->at(row * m1_cols + k) * m2->at(k * m2_cols + col);
            }
        }
    }

    return res;
}

vector<double>* NeuralNet::loss(vector<double>* vec) {
    int len = vec->size();

    vector<double>* res = new vector<double>();

    for (int i = 0; i < len; i++) {
        res->push_back(1 / (1 + exp(-vec->at(i))));
    }

    return res;
}

vector<double>* NeuralNet::sub(vector<double>* m1, vector<double>* m2) {
    int len = m1->size();
    
    vector<double>* res = new vector<double>();

    for (int i = 0; i < len; i++) {
        res->push_back(m1->at(i) - m2->at(i));
    }

    return res;
}



vector<double>* NeuralNet::add(vector<double>* m1, vector<double>* m2) {
    int len = m1->size();
    
    vector<double>* res = new vector<double>();

    for (int i = 0; i < len; i++) {
        res->push_back(m1->at(i) + m2->at(i));
    }

    return res;
}

vector<double>* NeuralNet::elem_mult(vector<double>* m1, vector<double>* m2) {
    int len = m1->size();

    vector<double>* res = new vector<double>();

    for (int i = 0; i < len; i++) {
        res->push_back(m1->at(i) * m2->at(i));
    }

    return res;
}

vector<double>* NeuralNet::scalar_mult(int s, vector<double>* m) {
    vector<double>* res = new vector<double>();

    for (int i = 0; i < m->size(); i++) {
        res->push_back(s * m->at(i));
    }

    return res;
}


vector<double>* NeuralNet::loss_d(vector<double>* vec) {
    int len = vec->size();

    vector<double>* res = new vector<double>();

    for (int i = 0; i < len; i++) {
        res->push_back(vec->at(i) * (1 - vec->at(i))); 
    }

    return res;
}

vector<double>* NeuralNet::transpose(vector<double>* m, int cols, int rows) {
    vector<double>* res = new vector<double>();
    int n, i, j;

    for (i = 0; i < cols * rows; i++) {
        res->push_back(0);
    }

    for (n = 0; n < cols * rows; n++) {
        i = n / cols;
        j = n % cols;

        res->at(rows * j + i) = m->at(n);
    }
    
    return res;
}

vector<vector<double>*>* NeuralNet::think(vector<double>* X) {
    int i, inputs_per_node, nodes;

    vector<vector<double>*>* outputs;
    vector<double>* curr_output; 
    vector<double>* prev_output; 
    vector<double>* weights;
    vector<double>* prod;
    NeuronLayer* curr_layer;

    outputs = new vector<vector<double>*>();

    for (i = 0; i < this->num_layers; i++) {
        if (i > 0) {
            prev_output = outputs->at(i - 1);
        } else {
            prev_output = X;
        }

        curr_layer = this->layers->at(i);
        inputs_per_node = curr_layer->inputs_per_node;
        nodes = curr_layer->nodes;
        weights = curr_layer->weights;

        prod = mat_mult(prev_output, weights, 4, inputs_per_node, nodes); 
        curr_output = loss(prod);
        outputs->push_back(curr_output);
    }
    
    return outputs;
}


void NeuralNet::train_network(vector<double>* X, vector<double>* y, int epochs) {
    int i, epoch;

    vector<vector<double>*>* outputs;
    vector<vector<double>*>* layer_errors;
    vector<vector<double>*>* layer_deltas;
    
    vector<double>* lr_vec;
    vector<double>* layer_error; 
    vector<double>* layer_delta;
    vector<double>* prev_delta;
    vector<double>* prev_weights;
    vector<double>* W_delta;

    NeuronLayer* layer; 
    NeuronLayer* prev_layer;

    for (epoch = 0; epoch < epochs; epoch++) {
        outputs = think(X);
        layer_deltas = new vector<vector<double>*>();
        
        // Calculating errors and deltas for every layer
        for (i = this->num_layers - 1; i > -1; i--) {
            if (i == this->num_layers - 1) {          // Output layer
                layer_error = sub(y, outputs->at(i));
            } else {
                prev_layer = this->layers->at(i + 1);
                prev_delta = layer_deltas->back();
                prev_weights = transpose(prev_layer->weights, 
                                prev_layer->inputs_per_node, prev_layer->nodes);
                layer_error = mat_mult(prev_delta, prev_weights, 
                               4, prev_layer->nodes, prev_layer->inputs_per_node); 
            }

            layer_delta = elem_mult(layer_error, loss_d(outputs->at(i)));
            layer_deltas->push_back(layer_delta);
        }

        Utils::reverseVec(layer_deltas);

        // Adjust the weights
        for (i = 0; i < this->num_layers; i++) {
            layer = this->layers->at(i);
            layer_delta = layer_deltas->at(i);

            if (i == 0) { // Input layer
                W_delta = mat_mult(transpose(X, 4, 4), layer_delta, 4, 
                                layer->inputs_per_node, layer->nodes);
            } else if (i == this->num_layers - 1) { // Output layer
                W_delta = mat_mult(transpose(outputs->at(i - 1), 4, 2), layer_delta, 2, 4, 1);
            } else { // Hidden layer
                W_delta = mat_mult(transpose(outputs->at(i - 1), 4, 2), layer_delta, 2, 4, 2);
            }

            
            W_delta = scalar_mult(this->learning_rate, W_delta);
            layer->weights = add(layer->weights, W_delta);            
        }
    }
}

