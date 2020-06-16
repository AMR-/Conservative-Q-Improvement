#include "../include/leafsplit.hpp"

LeafSplit::LeafSplit(int feature, double value, vector<double>* leftQS, 
    vector<double>* rightQS, vector<double>* leftQAS, vector<double>* rightQAS, 
    vector<double>* leftQBS, vector<double>* rightQBS, vector<double>* leftQCS, vector<double>* rightQCS, 
    double leftVisits, double rightVisits) {
    this->feature = feature;
    this->value = value;

    this->leftQS = leftQS;
    this->rightQS = rightQS;
    this->leftQAS = leftQAS;
    this->rightQAS = rightQAS;
    this->leftQBS = leftQBS;
    this->rightQBS = rightQBS;
    this->leftQCS = leftQCS;
    this->rightQCS = rightQCS;

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

void LeafSplit::updateA(State* s, Action* a, int target, unordered_map<string, 
    double>* params) {

    double visitDecay = params->at("visitDecay");
    double alpha = params->at("alpha");
    
    this->leftVisits = this->leftVisits * visitDecay;
    this->rightVisits = this->rightVisits * visitDecay;

    if (s->state->at(this->feature) < this->value) {
        this->leftQAS->at(a->value) = (1 - alpha) * this->leftQAS->at(a->value) + alpha * target;
        this->leftVisits = this->leftVisits + (1 - visitDecay);
    } else {
        this->rightQAS->at(a->value) = (1 - alpha) * this->rightQAS->at(a->value) + alpha * target;
        this->rightVisits = this->rightVisits + (1 - visitDecay);
    }
}

void LeafSplit::updateB(State* s, Action* a, int target, unordered_map<string, 
    double>* params) {

    double visitDecay = params->at("visitDecay");
    double alpha = params->at("alpha");
    
    this->leftVisits = this->leftVisits * visitDecay;
    this->rightVisits = this->rightVisits * visitDecay;

    if (s->state->at(this->feature) < this->value) {
        this->leftQBS->at(a->value) = (1 - alpha) * this->leftQBS->at(a->value) + alpha * target;
        this->leftVisits = this->leftVisits + (1 - visitDecay);
    } else {
        this->rightQBS->at(a->value) = (1 - alpha) * this->rightQBS->at(a->value) + alpha * target;
        this->rightVisits = this->rightVisits + (1 - visitDecay);
    }
}

void LeafSplit::updateC(State* s, Action* a, int target, unordered_map<string, 
    double>* params) {

    double visitDecay = params->at("visitDecay");
    double alpha = params->at("alpha");
    
    this->leftVisits = this->leftVisits * visitDecay;
    this->rightVisits = this->rightVisits * visitDecay;

    if (s->state->at(this->feature) < this->value) {
        this->leftQCS->at(a->value) = (1 - alpha) * this->leftQCS->at(a->value) + alpha * target;
        this->leftVisits = this->leftVisits + (1 - visitDecay);
    } else {
        this->rightQCS->at(a->value) = (1 - alpha) * this->rightQCS->at(a->value) + alpha * target;
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

double LeafSplit::evalUtilityA(vector<double>* polQVals) {
    int actionChosen = Utils::argmax(polQVals);

    double leftQASMax = Utils::max(this->leftQAS);
    double rightQASMax = Utils::max(this->rightQAS);
    
    double leftPotUtil = leftQASMax - this->leftQAS->at(actionChosen);
    double rightPotUtil = rightQASMax - this->rightQAS->at(actionChosen);
    
    return leftPotUtil * this->leftVisits + rightPotUtil * this->rightVisits;
}

double LeafSplit::evalUtilityB(vector<double>* polQVals) {
    int actionChosen = Utils::argmax(polQVals);

    double leftQBSMax = Utils::max(this->leftQBS);
    double rightQBSMax = Utils::max(this->rightQBS);
    
    double leftPotUtil = leftQBSMax - this->leftQBS->at(actionChosen);
    double rightPotUtil = rightQBSMax - this->rightQBS->at(actionChosen);
    
    return leftPotUtil * this->leftVisits + rightPotUtil * this->rightVisits;
}

double LeafSplit::evalUtilityC(vector<double>* polQVals) {
    int actionChosen = Utils::argmax(polQVals);

    double leftQCSMax = Utils::max(this->leftQCS);
    double rightQCSMax = Utils::max(this->rightQCS);
    
    double leftPotUtil = leftQCSMax - this->leftQBS->at(actionChosen);
    double rightPotUtil = rightQCSMax - this->rightQBS->at(actionChosen);
    
    return leftPotUtil * this->leftVisits + rightPotUtil * this->rightVisits;
}
