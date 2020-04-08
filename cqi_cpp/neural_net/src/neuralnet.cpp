#include "../include/neuralnet.hpp"

NeuralNet::NeuralNet(double learning_rate, int layers, int nodes_per_layer) {
    this->learning_rate = learning_rate;
    this->layers = layers;
    this->nodes_per_layer = nodes_per_layer;
}

vector<double>* 
NeuralNet::dot(vector<double>* m1, vector<double>* m2, int m1_rows, int m1_cols, int m2_cols) {
    vector<double>* res = new vector<double>();
    int row, col, k;

    for (row = 0; row < m1_rows; row++) {
        for (col = 0; col < m2_cols; col++) {
            res->push_back(0.0);
        }
    }

    for (row = 0; row < m1_rows; row++) {
        for (col = 0; col < m2_cols; col++) {
            res->at(row * m2_cols + col) = 0.0;
            for (k = 0; k < m1_cols; k++) {
                res->at(row * m2_cols + col) += m1->at(row * m1_cols + k) * m2->at(k * m2_cols + col);
            }
        }
    }

    return res;
}

vector<double>* NeuralNet::sigmoid(vector<double>* vec) {
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

vector<double>* NeuralNet::sigmoid_d(vector<double>* vec) {
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

void NeuralNet::print_matrix(vector<double>* m, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cout << m->at(i * cols + j) << " ";
        }

        cout << "\n";
    }

    cout << endl;
}

vector<double>* NeuralNet::random_matrix(int rows, int cols) {
    vector<double>* res = new vector<double>();

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(-1.0, 1.0);

    for (int i = 0; i < rows * cols; i++) {
       res->push_back(dis(gen));
    }

    return res;
}


int main() {
    vector<double>* X;
    vector<double>* y;
    vector<double>* W;
    int i;

    vector<double> X_vals {
        5.1, 3.5, 1.4, 0.2,
        4.9, 3.0, 1.4, 0.2, 
        6.2, 3.4, 5.4, 2.3,
        5.9, 3.0, 5.1, 1.8
    };

    X = &X_vals;

    vector<double> y_vals {
        0,
        0,
        1,
        1
    };

    y = &y_vals;

    vector<double>* pred;
    vector<double>* pred_error;
    vector<double>* pred_delta;
    vector<double>* W_delta;
    vector<double>* lr_vec = new vector<double>();

    NeuralNet *nn = new NeuralNet(0.01, 1, 1);

    for (i = 0; i < 4 * nn->layers; i++) {
        lr_vec->push_back(nn->learning_rate);
    }

    W = nn->random_matrix(4, nn->layers);

    for (i = 0; i < 5000; i++) {
        pred = nn->sigmoid(nn->dot(X, W, 4, 4, nn->layers));
        pred_error = nn->sub(y, pred);
        pred_delta = nn->elem_mult(pred_error, nn->sigmoid_d(pred));
        W_delta = nn->dot(nn->transpose(X, 4, 4), pred_delta, 4, 4, 1);
        W_delta = nn->elem_mult(lr_vec, W_delta);
        W = nn->add(W, W_delta);
    }

    nn->print_matrix(pred, 4, 1);

    return 0;
}


