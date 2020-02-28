#include "../include/leafsplit.hpp"

LeafSplit::LeafSplit(int feature, double value, vector<double>* leftQS, 
    vector<double>* rightQS, double leftVisits, double rightVisits) {
    
    this->feature = feature;
    this->value = value;
    this->leftQS = leftQS;
    this->rightQS = rightQS;
    this->leftVisits = leftVisits;
    this->rightVisits = rightVisits;
}

void LeafSplit::update(State* s, Action* a, int target, unordered_map<string, 
    double>* params) {

    double visitDecay = params->at("visitDecay");
    double alpha = params->at("alpha");
    
    this->leftVisits = this->leftVisits * visitDecay;
    this->rightVisits = this->rightVisits * visitDecay;

    if (s->state->at(this->feature) < this->value) {
        this->leftQS->at(a->value) = (1 - alpha) * this->leftQS->at(a->value) + alpha * target;
        this->leftVisits = this->leftVisits + (1 - visitDecay);
    } else {
        this->rightQS->at(a->value) = (1 - alpha) * this->rightQS->at(a->value) + alpha * target;
        this->rightVisits = this->rightVisits + (1 - visitDecay);
    }
}

double LeafSplit::evalUtility(vector<double>* polQVals) {
    int actionChosen = Utils::argmax(polQVals);

    double leftQSMax = Utils::max(this->leftQS);
    double rightQSMax = Utils::max(this->rightQS);
    
    double leftPotUtil = leftQSMax - this->leftQS->at(actionChosen);
    double rightPotUtil = rightQSMax - this->rightQS->at(actionChosen);
    
    return leftPotUtil * this->leftVisits + rightPotUtil * this->rightVisits;
}
