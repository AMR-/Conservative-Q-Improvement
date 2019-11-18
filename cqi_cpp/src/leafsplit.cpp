#include "../include/leafsplit.hpp"

LeafSplit::LeafSplit(int feature, float value, vector<float>* leftQS, vector<float>* rightQS, float leftVisits, float rightVisits) {
    this->feature = feature;
    this->value = value;
    this->leftQS = leftQS;
    this->rightQS = rightQS;
    this->leftVisits = leftVisits;
    this->rightVisits = rightVisits;
}

void LeafSplit::update(State* s, Action* a, int target, unordered_map<string, float>* params) {
    this->leftVisits = this->leftVisits * params->at("visitDecay");
    this->rightVisits = this->rightVisits * params->at("visitDecay");

    if (s->state->at(this->feature) < this->value) {
        this->leftQS->at(a->value) = (1 - params->at("alpha")) * this->leftQS->at(a->value) + params->at("alpha") * target;
        this->leftVisits = this->leftVisits + (1 - params->at("visitDecay"));
    } else {
        this->rightQS->at(a->value) = (1 - params->at("alpha")) * this->rightQS->at(a->value) + params->at("alpha") * target;
        this->rightVisits = this->rightVisits + (1 - params->at("visitDecay"));
    }
}

float LeafSplit::evalUtility(vector<float>* polQVals) {
    int actionChosen = Utils::vectorArgmax(polQVals);

    auto leftQSMax = max_element(begin(*this->leftQS), end(*this->leftQS));
    auto rightQSMax = max_element(begin(*this->rightQS), end(*this->rightQS));

    float leftPotUtil = *leftQSMax - this->leftQS->at(actionChosen);
    float rightPotUtil = *rightQSMax - this->rightQS->at(actionChosen);
    
    return leftPotUtil * this->leftVisits + rightPotUtil * this->rightVisits;
}
