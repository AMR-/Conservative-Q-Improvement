#include "../include/qtreeinternal.hpp"

QTreeInternal::QTreeInternal(QTreeNode* leftChild, QTreeNode* rightChild, int feature, 
    float value, float visits) : QTreeNode(visits) {
    
    this->leftChild = leftChild;
    this->rightChild = rightChild;
    this->feature = feature;
    this->value = value;
}

bool QTreeInternal::isLeaf() {
    return false;
}

vector<float>* QTreeInternal::getQS(State* s) {
    return this->selectChild(s).first->getQS(s); 
}

void QTreeInternal::update(State* s, Action* a, int target, unordered_map<string, float>* 
    params) {
    
    QTreeNode::update(s, a, target, params);
    pair<QTreeNode*, QTreeNode*> selectPair = this->selectChild(s);
    QTreeNode* it = selectPair.first;
    QTreeNode* notIt = selectPair.second;
    it->update(s, a, target, params);
    notIt->noVisitUpdate(params);
}

QTreeInternal* QTreeInternal::split(State* s, vector<float>* boxLow, vector<float>* 
    boxHigh, unordered_map<string, float>* params) {
    
    if (s->state->at(this->feature) < this->value) {
        boxHigh->at(this->feature) = this->value;
        this->leftChild = this->leftChild->split(s, boxLow, boxHigh, params);
    } else {
        boxLow->at(this->feature) = this->value;
        this->rightChild = this->rightChild->split(s, boxLow, boxHigh, params);
    } 
            
    return this;
}

pair<QTreeNode*, QTreeNode*> QTreeInternal::selectChild(State* s) {
    pair<QTreeNode*, QTreeNode*> selectPair(this->leftChild, this->rightChild); 
            
    if (s->state->at(this->feature) >= this->value) {
        selectPair.first = this->rightChild;
        selectPair.second = this->leftChild;
    } 

    return selectPair;
}

float QTreeInternal::maxSplitUntil(State* s) {
    return this->visits * this->selectChild(s).first->maxSplitUntil(s);
}

int QTreeInternal::numNodes() {
    return 1 + this->leftChild->numNodes() + this->rightChild->numNodes();
}

void QTreeInternal::printStructure(string prefixHead, string prefixTail) {
    printf("%s (vis: %1.2f) if f[%d] ? %f:", prefixHead.c_str(), this->visits, this->feature, 
        this->value); 

    string leftChildFirst = prefixTail + "  ├<";
    string leftChildSecond = prefixTail + " │";
    string rightChildFirst = prefixTail + " └>";
    string rightChildSecond = prefixTail + "  ";

    this->leftChild->printStructure(leftChildFirst, leftChildSecond);
    this->rightChild->printStructure(rightChildFirst, rightChildSecond);
}
