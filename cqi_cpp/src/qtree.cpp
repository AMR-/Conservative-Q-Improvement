#include "../include/qtree.hpp"
#include <iostream>
typedef unordered_map<string, double> map;

QTree::QTree(Box* stateSpace, Discrete* actionSpace, QTreeNode* root=nullptr, 
    double gamma=0.99, double alpha=0.1, double visitDecay=0.99, double splitThreshMax=1, double 
    splitThreshDecay=0.99, int numSplits=2, int cql=0) : QFunc(stateSpace, actionSpace) {
   
    if (!root) {
        vector<double>* low = this->stateSpace->low;
        vector<double>* high = this->stateSpace->high;
        vector<LeafSplit*>* splits = new vector<LeafSplit*>(); 
        int sz = actionSpace->size();
        int numOfFuncs = 6;

        for (size_t f = 0; f < low->size(); f++) {
            for (int i = 0; i < numSplits; i++) {
                vector<double>* zeros = Utils::zeros(sz);
                vector<vector<double>*>* twoDZeros = Utils::twoDZeros(numOfFuncs, sz);
                int value = low->at(f) + (high->at(f) - low->at(f))/(numSplits + 1) * (i + 1);
                LeafSplit* toAdd = new LeafSplit(f, value, zeros, zeros, twoDZeros, twoDZeros, 0.5, 0.5);
                splits->push_back(toAdd);
            }
        }

        this->root = new QTreeLeaf(Utils::zeros(sz), Utils::twoDZeros(numOfFuncs, sz), 1, splits);
    } else {
        this->root = root;
    }

    this->params = new map();
    this->params->insert(map::value_type("gamma", gamma));
    this->params->insert(map::value_type("alpha", alpha));
    this->params->insert(map::value_type("visitDecay", visitDecay));
    this->params->insert(map::value_type("numSplits", numSplits));
    this->params->insert(map::value_type("cql_update", 0));
    
    this->splitThreshMax = splitThreshMax;
    this->splitThreshDecay = splitThreshDecay;
    this->splitThresh = this->splitThreshMax;
    this->_justSplit = false;  // True if the most recent action resulted in a split
    this->cql = cql;
}

int QTree::selectA(State* s) {
    return Utils::argmax(this->root->getQS(s));
}

int QTree::selectAWithCQL(State* s) {
    srand(time(NULL));

    int r = rand() % 4;
    int a = 0;

    // Update three of the six Q-functions
    this->params->at("cql_update") = r;
    
    r = rand() % 6;
    // Get argmax of one of the six Q-functions
    a = Utils::argmax(this->root->getQS(s, r)); 

    return a;
}

void QTree::takeTuple(State* s, Action* a, double r, State* s2, bool done) {
    this->_justSplit = false;
    this->selfCopy = NULL;
    double msu = 0, msu_two = 0, msu_three = 0;
    int cql_update = (int) this->params->at("cql_update");
    vector<double>* vals = new vector<double>();

    // update a leaf directly
    if (this->cql) {
        switch (cql_update) {
            case 0:
                this->update(s, a, r, s2, done, 0);
                this->update(s, a, r, s2, done, 1);
                this->update(s, a, r, s2, done, 2);
                msu = this->root->maxSplitUtil(s, 0);
                msu_two = this->root->maxSplitUtil(s, 1);
                msu_three = this->root->maxSplitUtil(s, 2);
                break;
            case 1:
                this->update(s, a, r, s2, done, 0);
                this->update(s, a, r, s2, done, 3);
                this->update(s, a, r, s2, done, 4);
                msu = this->root->maxSplitUtil(s, 0);
                msu_two = this->root->maxSplitUtil(s, 3);
                msu_three = this->root->maxSplitUtil(s, 4);
                break;
            case 2:
                this->update(s, a, r, s2, done, 1);
                this->update(s, a, r, s2, done, 3);
                this->update(s, a, r, s2, done, 5);
                msu = this->root->maxSplitUtil(s, 1);
                msu_two = this->root->maxSplitUtil(s, 3);
                msu_three = this->root->maxSplitUtil(s, 5);
                break;
            default:
                this->update(s, a, r, s2, done, 1);
                this->update(s, a, r, s2, done, 4);
                this->update(s, a, r, s2, done, 5);
                msu = this->root->maxSplitUtil(s, 1);
                msu_two = this->root->maxSplitUtil(s, 4);
                msu_three = this->root->maxSplitUtil(s, 5);
        }
    } else {
        this->update(s, a, r, s2, done);
        msu = this->root->maxSplitUtil(s);
    }
    
    vals->push_back(msu);
    vals->push_back(msu_two);
    vals->push_back(msu_three);

    msu = Utils::max(vals);
    
    // modify tree
    if (msu > this->splitThresh) {
        printf("split\n");

        this->_justSplit = true;

        if (this->makeCopies) {
            this->selfCopy = new QTree(this->stateSpace, this->actionSpace, 
                this->root, this->params->at("gamma"), this->params->at("alpha"), 
                this->params->at("visitDecay"), this->splitThreshMax, 
                this->splitThreshDecay, this->params->at("numSplits"));
        }

        this->root = this->root->split(s, this->stateSpace->low, this->stateSpace->high, this->params);
        this->splitThresh = this->splitThreshMax;
    } else {
        this->splitThresh = this->splitThresh * this->splitThreshDecay;
    }
}

void QTree::update(State* s, Action* a, double r, State* s2, bool done) {
    double target = 0;

    if (done) {
        target = r;
    } else {
        vector<double>* QVals = this->root->getQS(s2);
        double QValsMax = Utils::max(QVals);

        target = r + this->params->at("gamma") * QValsMax;
    }

    this->root->update(s, a, target, this->params);
}

void QTree::update(State* s, Action* a, double r, State* s2, bool done, int index) {
    double target = 0;

    if (done) {
        target = r;
    } else {
        vector<double>* QVals = this->root->getQS(s2, (index + 1) % 6);
        double QValsMax = Utils::max(QVals);

        target = r + this->params->at("gamma") * QValsMax;
    }

    this->root->update(s, a, target, this->params, index);
}

int QTree::numNodes() {
    return this->root->numNodes();
}

void QTree::printStructure() {
    this->root->printStructure("└", " ");
}
        
bool QTree::justSplit() {
    return this->_justSplit;
}
