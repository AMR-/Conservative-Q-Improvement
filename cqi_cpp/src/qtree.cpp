#include "../include/qtree.hpp"
#include <iostream>
typedef unordered_map<string, double> map;

QTree::QTree(Box* stateSpace, Discrete* actionSpace, QTreeNode* root=nullptr, 
    double gamma=0.99, double alpha=0.1, double visitDecay=0.99, double splitThreshMax=1, double 
    splitThreshDecay=0.99, int numSplits=2, int c=2, int mql=0) : QFunc(stateSpace, actionSpace) {
   
    if (!root) {
        vector<double>* low = this->stateSpace->low;
        vector<double>* high = this->stateSpace->high;
        vector<LeafSplit*>* splits = new vector<LeafSplit*>(); 

        for (size_t f = 0; f < low->size(); f++) {
            for (int i = 0; i < numSplits; i++) {
                vector<double>* zeros = Utils::zeros(actionSpace->size());
                int value = low->at(f) + (high->at(f) - low->at(f))/(numSplits + 1) * (i + 1);
                LeafSplit* toAdd = new LeafSplit(f, value, zeros, zeros, 
                                zeros, zeros, zeros, zeros, 0.5, 0.5);
                
                splits->push_back(toAdd);
            }
        }

        this->root = new QTreeLeaf(Utils::zeros(actionSpace->size()), 
            Utils::zeros(actionSpace->size()), Utils::zeros(actionSpace->size()), 1, splits);
    } else {
        this->root = root;
    }

    this->params = new map();
    this->params->insert(map::value_type("gamma", gamma));
    this->params->insert(map::value_type("alpha", alpha));
    this->params->insert(map::value_type("visitDecay", visitDecay));
    this->params->insert(map::value_type("numSplits", numSplits));
    this->params->insert(map::value_type("mql_action", 0));
    
    this->splitThreshMax = splitThreshMax;
    this->splitThreshDecay = splitThreshDecay;
    this->splitThresh = this->splitThreshMax;
    this->_justSplit = false;  // True if the most recent action resulted in a split
    this->c = c;
    this->timesteps = 0;
    this->frequencies = Utils::zeros(actionSpace->size());
    this->mql = mql;
}

int QTree::selectA(State* s) {
    return Utils::argmax(this->root->getQS(s));
}

int QTree::selectAWithUCB(State* s) {
    vector<double>* vecForUCB = new vector<double>();
    int t = this->timesteps;
    vector<double>* N = this->frequencies;

    for (size_t i = 0; i < N->size(); i++) {
        if (N->at(i) == 0) {
            return i;
        } else {
            vecForUCB->push_back(this->root->getQS(s)->at(i) + this->c * sqrt(log(t)/(N->at(i))));
        }
    }

    return Utils::argmax(vecForUCB);
}

int QTree::selectAWithMQL(State* s) {
    srand(time(NULL));

    int r = rand() % 2;
    int a = 0;

    if (r == 0) {
        a = Utils::argmax(this->root->getQAS(s)); 
        this->params->at("mql_action") = 0;
    } else {
        a = Utils::argmax(this->root->getQBS(s)); 
        this->params->at("mql_action") = 1;
    }
    
    return a;
}

void QTree::incrementVals(Action* a) {
    this->timesteps += 1;
    this->frequencies->at(a->value) = this->frequencies->at(a->value) + 1;
}

void QTree::takeTuple(State* s, Action* a, double r, State* s2, bool done) {
    this->_justSplit = false;
    this->selfCopy = NULL;
    double msu = 0;

    // update a leaf directly
    if (this->mql) {
        if (this->params->at("mql_action") == 0) {
            this->updateA(s, a, r, s2, done);
            msu = this->root->maxSplitUtilA(s);
        } else {
            this->updateB(s, a, r, s2, done);
            msu = this->root->maxSplitUtilB(s);
        }
    } else {
        this->update(s, a, r, s2, done);
        msu = this->root->maxSplitUtil(s);
    }

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

        this->root = this->root->split(s, this->stateSpace->low, 
                        this->stateSpace->high, this->params);
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

void QTree::updateA(State* s, Action* a, double r, State* s2, bool done) {
    double target = 0;

    if (done) {
        target = r;
    } else {
        vector<double>* QVals = this->root->getQBS(s2);
        double QValsMax = Utils::max(QVals);

        target = r + this->params->at("gamma") * QValsMax;
    }

    this->root->updateA(s, a, target, this->params);
}

void QTree::updateB(State* s, Action* a, double r, State* s2, bool done) {
    double target = 0;

    if (done) {
        target = r;
    } else {
        vector<double>* QVals = this->root->getQAS(s2);
        double QValsMax = Utils::max(QVals);

        target = r + this->params->at("gamma") * QValsMax;
    }

    this->root->updateB(s, a, target, this->params);
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
