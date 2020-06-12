#include "../include/qtreeinternal.hpp"
#include <iostream>

QTreeInternal::QTreeInternal(QTreeNode* leftChild, QTreeNode* rightChild, int feature, 
    double value, double visits) : QTreeNode(visits) {
    
    this->leftChild = leftChild;
    this->rightChild = rightChild;
    this->feature = feature;
    this->value = value;
}

bool QTreeInternal::isLeaf() {
    return false;
}

vector<double>* QTreeInternal::getQS(State* s) {
    return get<0>(this->selectChild(s))->getQS(s); 
}

vector<double>* QTreeInternal::getQAS(State* s) {
    return get<0>(this->selectChild(s))->getQAS(s); 
}

vector<double>* QTreeInternal::getQBS(State* s) {
    return get<0>(this->selectChild(s))->getQBS(s); 
}

void QTreeInternal::update(State* s, Action* a, double target, unordered_map<string, double>* 
    params) {
    
    this->visits = this->visits * params->at("visitDecay") + (1 - params->at("visitDecay"));
    tuple<QTreeNode*, QTreeNode*> selectPair = this->selectChild(s);
    QTreeNode* it = get<0>(selectPair);
    QTreeNode* notIt = get<1>(selectPair);

    it->update(s, a, target, params);
    notIt->noVisitUpdate(params);
}

void QTreeInternal::updateA(State* s, Action* a, double target, unordered_map<string, double>* 
    params) {
    
    this->visits = this->visits * params->at("visitDecay") + (1 - params->at("visitDecay"));
    tuple<QTreeNode*, QTreeNode*> selectPair = this->selectChild(s);
    QTreeNode* it = get<0>(selectPair);
    QTreeNode* notIt = get<1>(selectPair);

    it->updateA(s, a, target, params);
    notIt->noVisitUpdate(params);
}

void QTreeInternal::updateB(State* s, Action* a, double target, unordered_map<string, double>* 
    params) {
    
    this->visits = this->visits * params->at("visitDecay") + (1 - params->at("visitDecay"));
    tuple<QTreeNode*, QTreeNode*> selectPair = this->selectChild(s);
    QTreeNode* it = get<0>(selectPair);
    QTreeNode* notIt = get<1>(selectPair);

    it->updateB(s, a, target, params);
    notIt->noVisitUpdate(params);
}

QTreeInternal* QTreeInternal::split(State* s, vector<double>* boxLow, vector<double>* 
    boxHigh, unordered_map<string, double>* params) {
    
    if (s->state->at(this->feature) < this->value) {
        boxHigh->at(this->feature) = this->value;
        this->leftChild = this->leftChild->split(s, boxLow, boxHigh, params);
    } else {
        boxLow->at(this->feature) = this->value;
        this->rightChild = this->rightChild->split(s, boxLow, boxHigh, params);
    } 
            
    return this;
}

void QTreeInternal::noVisitUpdate(unordered_map<string, double>* params) {
    this->visits = this->visits * params->at("visitDecay");
}

tuple<QTreeNode*, QTreeNode*> QTreeInternal::selectChild(State* s) {
    if (s->state->at(this->feature) < this->value) {
        return tuple<QTreeNode*, QTreeNode*>(this->leftChild, this->rightChild);  
    } else {
        return tuple<QTreeNode*, QTreeNode*>(this->rightChild, this->leftChild);  
    }
}

double QTreeInternal::maxSplitUtil(State* s) {
    return this->visits * get<0>(this->selectChild(s))->maxSplitUtil(s);
}

double QTreeInternal::maxSplitUtilA(State* s) {
    return this->visits * get<0>(this->selectChild(s))->maxSplitUtilA(s);
}

double QTreeInternal::maxSplitUtilB(State* s) {
    return this->visits * get<0>(this->selectChild(s))->maxSplitUtilB(s);
}

int QTreeInternal::numNodes() {
    return 1 + this->leftChild->numNodes() + this->rightChild->numNodes();
}

void QTreeInternal::printStructure(string prefixHead, string prefixTail) {
    printf("%s (vis: %1.2f) if f[%d] ? %f:\n", prefixHead.c_str(), this->visits, this->feature, 
        this->value); 

    string leftChildFirst = prefixTail + " ├<";
    string leftChildSecond = prefixTail + " │";
    string rightChildFirst = prefixTail + " └>";
    string rightChildSecond = prefixTail + "  ";

    this->leftChild->printStructure(leftChildFirst, leftChildSecond);
    this->rightChild->printStructure(rightChildFirst, rightChildSecond);
}
