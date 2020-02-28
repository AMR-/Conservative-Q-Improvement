#include "../include/qtreeleaf.hpp"
#include <iostream>

QTreeLeaf::QTreeLeaf(vector<double>* qs, double visits, vector<LeafSplit*>* splits) 
    : QTreeNode(visits) {
    
    this->qs = qs;
    this->splits = splits;
}

bool QTreeLeaf::isLeaf() {
    return true;
}

vector<double>* QTreeLeaf::getQS(State* s) {
    return this->qs;
}
        
void QTreeLeaf::update(State* s, Action* a, double target, unordered_map<string, double>* params) {
    this->visits = this->visits * params->at("visitDecay") + (1 - params->at("visitDecay"));
    double alpha = params->at("alpha");
    this->qs->at(a->value) = (1 - alpha) * this->qs->at(a->value) + alpha * target;
    
    for (auto& split : *(this->splits))
        split->update(s, a, target, params);
}

QTreeInternal* QTreeLeaf::split(State* s, vector<double>* boxLow, vector<double>* 
    boxHigh, unordered_map<string, double>* params) {
    
    vector<double>* splitUtils = new vector<double>();

    for (auto& sp: *(this->splits))
        splitUtils->push_back(sp->evalUtility(this->qs));

    int splitIndex = Utils::argmax(splitUtils);

    LeafSplit* sfSplit = this->splits->at(splitIndex);
    int splitFeature = sfSplit->feature;
    vector<LeafSplit*>* LSplits = new vector<LeafSplit*>(); 
    vector<LeafSplit*>* RSplits = new vector<LeafSplit*>(); 

    for (auto& sp: *(this->splits)) {
        if (sp->feature != splitFeature) {
            LSplits->push_back(new LeafSplit(sp->feature, sp->value, 
                Utils::copy(sfSplit->leftQS), Utils::copy(sfSplit->leftQS), 0.5, 0.5));
            RSplits->push_back(new LeafSplit(sp->feature, sp->value, 
                Utils::copy(sfSplit->rightQS), Utils::copy(sfSplit->rightQS), 0.5, 0.5));
        }
    }

    auto ns = params->at("numSplits");
    auto lowSF = boxLow->at(splitFeature);
    auto highSF = boxHigh->at(splitFeature);

    for (int i = 0; i < ns; i++) {
        LSplits->push_back(new LeafSplit(splitFeature, lowSF + (sfSplit->value - lowSF)/(ns + 1) * (i + 1), 
        Utils::copy(sfSplit->leftQS), Utils::copy(sfSplit->leftQS), 0.5, 0.5));

        RSplits->push_back(new LeafSplit(splitFeature, sfSplit->value + (highSF - sfSplit->value)/(ns + 1) * (i + 1), 
        Utils::copy(sfSplit->rightQS), Utils::copy(sfSplit->rightQS), 0.5, 0.5));
    }

    QTreeLeaf* leftChild = new QTreeLeaf(sfSplit->leftQS,  sfSplit->leftVisits, LSplits);
    QTreeLeaf* rightChild = new QTreeLeaf(sfSplit->rightQS, sfSplit->rightVisits, RSplits);
            
    double val = (highSF + lowSF) / 2;
    double visits = this->visits;
        
    return new QTreeInternal(leftChild, rightChild, splitFeature, val, visits);
}

double QTreeLeaf::maxSplitUtil(State* s) {
    vector<double>* evalUtilities = new vector<double>();

    for (auto& sp: *(this->splits))
        evalUtilities->push_back(sp->evalUtility(this->qs));

    double vectorMax = Utils::max(evalUtilities);
    
    return this->visits * vectorMax; 
}

void QTreeLeaf::noVisitUpdate(unordered_map<string, double>* params) {
    this->visits = this->visits * params->at("visitDecay");
}

int QTreeLeaf::numNodes() {
    return 1; 
}

void QTreeLeaf::printStructure(string prefixHead, string prefixTail) {
	string QString = Utils::vecToString(this->qs);

    printf("%s (vis: %1.2f) qvals: %s\n", prefixHead.c_str(), this->visits, QString.c_str());
}
