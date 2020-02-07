#include "../include/qtreeleaf.hpp"

QTreeLeaf::QTreeLeaf(vector<float>* qs, float visits, vector<LeafSplit*>* splits) 
    : QTreeNode(visits) {
    
    this->qs = qs;
    this->splits = splits;
}

bool QTreeLeaf::isLeaf() {
    return true;
}

vector<float>* QTreeLeaf::getQS(State* s) {
    return this->qs;
}
        
void QTreeLeaf::update(State* s, Action* a, int target, unordered_map<string, 
    float>* params) {
    
    QTreeNode::update(s, a, target, params);
    
    float alpha = params->at("alpha");
    this->qs->at(a->value) = (1 - alpha) * this->qs->at(a->value) + alpha * target;

    for (auto& split : *(this->splits))
        split->update(s, a, target, params);
}

QTreeInternal* QTreeLeaf::split(State* s, vector<float>* boxLow, vector<float>* 
    boxHigh, unordered_map<string, float>* params) {
    
    vector<float> splitUtils;

    for (auto& sp: *(this->splits))
        splitUtils.push_back(sp->evalUtility(this->qs));

    int splitIndex = Utils::argmax(&splitUtils);

    LeafSplit* sfSplit = this->splits->at(splitIndex);
    int splitFeature = sfSplit->feature;
    vector<LeafSplit*>* LSplits = new vector<LeafSplit*>(); 
    vector<LeafSplit*>* RSplits = new vector<LeafSplit*>(); 

    for (auto& sp: *(this->splits)) {
        if (sp->feature != splitFeature) {
            LSplits->push_back(new LeafSplit(sp->feature, sp->value, 
                sfSplit->leftQS, sfSplit->leftQS, 0.5, 0.5));
            RSplits->push_back(new LeafSplit(sp->feature, sp->value, 
                sfSplit->rightQS, sfSplit->rightQS, 0.5, 0.5));
        }
    }

    auto ns = params->at("numSplits");
    auto lowSF = boxLow->at(splitFeature);
    auto highSF = boxHigh->at(splitFeature);

    for (int i = 0; i < ns; i++) {
        LSplits->push_back(new LeafSplit(splitFeature, lowSF + (sfSplit->value - lowSF)/(ns + 1) * (i + 1), 
        sfSplit->leftQS, sfSplit->leftQS, 0.5, 0.5));

        RSplits->push_back(new LeafSplit(splitFeature, sfSplit->value + (highSF - sfSplit->value)/(ns + 1) * (i + 1), 
        sfSplit->rightQS, sfSplit->rightQS, 0.5, 0.5));
    }

    QTreeLeaf* leftChild = new QTreeLeaf(sfSplit->leftQS,  sfSplit->leftVisits, LSplits);
    QTreeLeaf* rightChild = new QTreeLeaf(sfSplit->rightQS, sfSplit->rightVisits, RSplits);
            
    float val = (highSF + lowSF) / 2;
    float visits = this->visits;
        
    return new QTreeInternal(leftChild, rightChild, splitFeature, val, visits);
}

float QTreeLeaf::maxSplitUntil(State* s) {
    vector<float> evalUtilities;

    for (auto& sp: *(this->splits))
        evalUtilities.push_back(sp->evalUtility(this->qs));

    auto vectorMax = max_element(begin(evalUtilities), end(evalUtilities));

    return this->visits * *vectorMax; 
}

int QTreeLeaf::numNodes() {
    return 1; 
}

void QTreeLeaf::printStructure(string prefixHead, string prefixTail) {
	string QString = Utils::vecToString(this->qs);

    printf("%s (vis: %1.2f) qvals: %s\n", prefixHead.c_str(), this->visits, QString.c_str());
}
