#include "../include/qtree.hpp"

QTree::QTree(vector<State*>* stateSpace, vector<Action*>* actionSpace, QTreeNode* root=NULL, float gamma=0.99, float alpha=0.1, float visitDecay=0.99, float splitThreshMax=1, float splitThreshDecay=0.99, int numSplits=2) : QFunc(stateSpace, actionSpace) {
    if (!root) {
        vector<float>* low = this->getLow(this->stateSpace);
        vector<float>* high = this->getHigh(this->stateSpace);
        vector<LeafSplit*>* splits;
               
        for (int f = 0; f < low->size(); f++) {
            for (int i = 0; i < numSplits; i++) {
                LeafSplit* toAdd = new LeafSplit(f, low->at(f) + (high->at(f) - low->at(f))/(numSplits+1)*(i+1), Utils::zeros(actionSpace->size()), Utils::zeros(actionSpace->size()), 0.5, 0.5);
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
       
QTree::QTree(const QTree &obj) : QFunc(stateSpace, actionSpace) {
    this->root = obj.root;
    this->params = obj.params;
    this->splitThreshMax = obj.splitThreshMax;
    this->splitThreshDecay = obj.splitThreshDecay;
    this->splitThresh = obj.splitThresh;
    this->_justSplit = obj._justSplit;
}

// TODO
vector<float>* QTree::getLow(vector<State*>* stateSpace) {
    return NULL;
}

// TODO
vector<float>* QTree::getHigh(vector<State*>* stateSpace) {
    return NULL;
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
            this->selfCopy = this; 
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
