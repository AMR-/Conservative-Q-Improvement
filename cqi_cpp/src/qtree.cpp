#include "../include/qtree.hpp"

// TODO: include Box, Discrete classes and implement methods
QTree::QTree(Box* stateSpace, Discrete* actionSpace, QTreeNode* root=nullptr, 
    float gamma=0.99, float alpha=0.1, float visitDecay=0.99, float splitThreshMax=1, float 
    splitThreshDecay=0.99, int numSplits=2) : QFunc(stateSpace, actionSpace) {
    
    if (!root) {
        vector<float>* low = this->getLow(this->stateSpace);
        vector<float>* high = this->getHigh(this->stateSpace);
        vector<LeafSplit*>* splits = new vector<LeafSplit*>(); 

        for (int f = 0; f < (int) low->size(); f++) {
            for (int i = 0; i < numSplits; i++) {
                vector<float>* zerosVector = Utils::zeros(actionSpace->size());

                LeafSplit* toAdd = new LeafSplit(f, low->at(f) + 
                    (high->at(f) - low->at(f))/(numSplits + 1) * (i + 1), zerosVector, 
                    zerosVector, 0.5, 0.5);
                
                splits->push_back(toAdd);
            }
        }

        this->root = new QTreeLeaf(Utils::zeros(actionSpace->size()), 1, splits);
    } else {
        this->root = root;
    }

    this->params = new unordered_map<string, float>(); 
    this->params->at("gamma") = gamma;
    this->params->at("alpha") = alpha;
    this->params->at("visitDecay") = visitDecay;
    this->params->at("numSplits") = numSplits;
    this->splitThreshMax = splitThreshMax;
    this->splitThreshDecay = splitThreshDecay;
    this->splitThresh = this->splitThreshMax;
    this->_justSplit = false;  // True if the most recent action resulted in a split
}

// TODO
vector<float>* QTree::getLow(Box* space) {
    return nullptr;
}

// TODO
vector<float>* QTree::getHigh(Box* space) {
    return nullptr;
}  

int QTree::selectA(State* s) {
    return Utils::argmax(this->root->getQS(s));
}

void QTree::takeTuple(State* s, Action* a, float r, State* s2, bool done) {
    this->_justSplit = false;
    this->selfCopy = NULL;
    // update a leaf directly
    this->update(s, a, r, s2, done);
    // modify tree
    if (this->root->maxSplitUntil(s) > this->splitThresh) {
        printf("split\n");

        this->_justSplit = true;

        if (this->makeCopies) {
            this->selfCopy = new QTree(this->stateSpace, this->actionSpace, 
                this->root, this->params->at("gamma"), this->params->at("alpha"), 
                this->params->at("visitDecay"), this->splitThreshMax, 
                this->splitThreshDecay, this->params->at("numSplits"));
        }

        this->root = this->root->split(s, this->getLow(this->stateSpace), this->getHigh(this->stateSpace), this->params);
        this->splitThresh = this->splitThreshMax;
    } else {
        this->splitThresh = this->splitThresh * this->splitThreshDecay;
    }
}

void QTree::update(State* s, Action* a, float r, State* s2, bool done) {
    float target;

    if (done) {
        target = r;
    } else {
        vector<float>* QVals = this->root->getQS(s2);
        auto QValsMax = max_element(begin(*QVals), end(*QVals));
        target = r + this->params->at("gamma") * *QValsMax;
        this->root->update(s, a, target, this->params);
    }
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
