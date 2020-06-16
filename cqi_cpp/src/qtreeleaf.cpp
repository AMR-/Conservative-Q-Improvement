#include "../include/qtreeleaf.hpp"
#include <iostream>
typedef vector<double> vec;

QTreeLeaf::QTreeLeaf(vec* qs, vec* qas, vec* qbs, vec* qcs, double visits, vector<LeafSplit*>* splits) 
    : QTreeNode(visits) {
    
    this->qs = qs;
    this->qas = qas;
    this->qbs = qbs;
    this->qcs = qcs;
    this->splits = splits;
}

bool QTreeLeaf::isLeaf() {
    return true;
}

vec* QTreeLeaf::getQS(State* s) {
    return this->qs;
}
        
vec* QTreeLeaf::getQAS(State* s) {
    return this->qas;
}

vec* QTreeLeaf::getQBS(State* s) {
    return this->qbs;
}

vec* QTreeLeaf::getQCS(State* s) {
    return this->qcs;
}

void QTreeLeaf::update(State* s, Action* a, double target, unordered_map<string, double>* params) {
    double vd = params->at("visitDecay");
    double alpha = params->at("alpha");

    this->visits = this->visits * vd + (1 - vd);
    this->qs->at(a->value) = (1 - alpha) * this->qs->at(a->value) + alpha * target;
    
    for (auto& split : *(this->splits))
        split->update(s, a, target, params);
}

void QTreeLeaf::updateA(State* s, Action* a, double target, unordered_map<string, double>* params) {
    double vd = params->at("visitDecay");
    double alpha = params->at("alpha");

    this->visits = this->visits * vd + (1 - vd);
    this->qas->at(a->value) = (1 - alpha) * this->qas->at(a->value) + alpha * target;
    
    for (auto& split : *(this->splits))
        split->updateA(s, a, target, params);
}

void QTreeLeaf::updateB(State* s, Action* a, double target, unordered_map<string, double>* params) {
    double vd = params->at("visitDecay");
    double alpha = params->at("alpha");

    this->visits = this->visits * vd + (1 - vd);
    this->qbs->at(a->value) = (1 - alpha) * this->qbs->at(a->value) + alpha * target;
    
    for (auto& split : *(this->splits))
        split->updateB(s, a, target, params);
}

void QTreeLeaf::updateC(State* s, Action* a, double target, unordered_map<string, double>* params) {
    double vd = params->at("visitDecay");
    double alpha = params->at("alpha");

    this->visits = this->visits * vd + (1 - vd);
    this->qcs->at(a->value) = (1 - alpha) * this->qcs->at(a->value) + alpha * target;
    
    for (auto& split : *(this->splits))
        split->updateC(s, a, target, params);
}

QTreeInternal* QTreeLeaf::split(State* s, vec* boxLow, vec* 
    boxHigh, unordered_map<string, double>* params) {
    
    vec* splitUtils = new vec();

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
                Utils::copy(sfSplit->leftQS), Utils::copy(sfSplit->leftQS), 
                Utils::copy(sfSplit->leftQAS), Utils::copy(sfSplit->leftQAS), 
                Utils::copy(sfSplit->leftQBS), Utils::copy(sfSplit->leftQBS), 
                Utils::copy(sfSplit->leftQCS), Utils::copy(sfSplit->leftQCS), 
                0.5, 0.5));
            RSplits->push_back(new LeafSplit(sp->feature, sp->value, 
                Utils::copy(sfSplit->rightQS), Utils::copy(sfSplit->rightQS), 
                Utils::copy(sfSplit->rightQAS), Utils::copy(sfSplit->rightQAS), 
                Utils::copy(sfSplit->rightQBS), Utils::copy(sfSplit->rightQBS), 
                Utils::copy(sfSplit->rightQCS), Utils::copy(sfSplit->rightQCS), 
                0.5, 0.5));
        }
    }

    auto ns = params->at("numSplits");
    auto lowSF = boxLow->at(splitFeature);
    auto highSF = boxHigh->at(splitFeature);

    for (int i = 0; i < ns; i++) {
        LSplits->push_back(new LeafSplit(splitFeature, 
                                lowSF + (sfSplit->value - lowSF)/(ns + 1) * (i + 1), 
        Utils::copy(sfSplit->leftQS), Utils::copy(sfSplit->leftQS), 
        Utils::copy(sfSplit->leftQAS), Utils::copy(sfSplit->leftQAS), 
        Utils::copy(sfSplit->leftQBS), Utils::copy(sfSplit->leftQBS), 
        Utils::copy(sfSplit->leftQCS), Utils::copy(sfSplit->leftQCS), 
        0.5, 0.5));

        RSplits->push_back(new LeafSplit(splitFeature, 
                                sfSplit->value + (highSF - sfSplit->value)/(ns + 1) * (i + 1), 
        Utils::copy(sfSplit->rightQS), Utils::copy(sfSplit->rightQS), 
        Utils::copy(sfSplit->rightQAS), Utils::copy(sfSplit->rightQAS), 
        Utils::copy(sfSplit->rightQBS), Utils::copy(sfSplit->rightQBS), 
        Utils::copy(sfSplit->rightQCS), Utils::copy(sfSplit->rightQCS), 
        0.5, 0.5));
    }

    QTreeLeaf* leftChild = new QTreeLeaf(sfSplit->leftQS, sfSplit->leftQAS, sfSplit->leftQBS, 
                    sfSplit->leftQCS, sfSplit->leftVisits, LSplits);
    QTreeLeaf* rightChild = new QTreeLeaf(sfSplit->rightQS, sfSplit->rightQAS, sfSplit->rightQBS, 
                    sfSplit->rightQCS, sfSplit->rightVisits, RSplits);
            
    double val = (highSF + lowSF) / 2;
    double visits = this->visits;
        
    return new QTreeInternal(leftChild, rightChild, splitFeature, val, visits);
}

double QTreeLeaf::maxSplitUtil(State* s) {
    vec* evalUtilities = new vec();

    for (auto& sp: *(this->splits))
        evalUtilities->push_back(sp->evalUtility(this->qs));

    double vectorMax = Utils::max(evalUtilities);
    
    return this->visits * vectorMax; 
}

double QTreeLeaf::maxSplitUtilA(State* s) {
    vec* evalUtilities = new vec();

    for (auto& sp: *(this->splits))
        evalUtilities->push_back(sp->evalUtilityA(this->qas));

    double vectorMax = Utils::max(evalUtilities);
    
    return this->visits * vectorMax; 
}

double QTreeLeaf::maxSplitUtilB(State* s) {
    vec* evalUtilities = new vec();

    for (auto& sp: *(this->splits))
        evalUtilities->push_back(sp->evalUtilityB(this->qbs));

    double vectorMax = Utils::max(evalUtilities);
    
    return this->visits * vectorMax; 
}

double QTreeLeaf::maxSplitUtilC(State* s) {
    vec* evalUtilities = new vec();

    for (auto& sp: *(this->splits))
        evalUtilities->push_back(sp->evalUtilityC(this->qbs));

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
