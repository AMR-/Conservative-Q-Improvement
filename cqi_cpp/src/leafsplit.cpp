#include "../include/leafsplit.hpp"

LeafSplit::LeafSplit(int feature, int value, vector<float>* leftQS, vector<float>* rightQS, int leftVisits, int rightVisits) {
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

    if (s->state[this->feature] < this->value) {
        this->leftQS->at(a->value) = (1 - params->at("alpha")) * this->leftQS->at(a->value) + params->at("alpha") * target;
        this->leftVisits = this->leftVisits + (1 - params->at("visitDecay"));
    } else {
        this->rightQS->at(a->value) = (1 - params->at("alpha")) * this->rightQS->at(a->value) + params->at("alpha") * target;
        this->rightVisits = this->rightVisits + (1 - params->at("visitDecay"));
    }
}

float vectorMax(vector<float>* vec) {
    float max = vec->at(0);

    for (auto& elem: *vec) {
        if (elem > max) max = elem;
    }

    return max;
}

int vectorArgmax(vector<float>* vec) {
    float max = vectorMax(vec);

    for (int i = 0; i < (int) vec->size(); i++) {
        if (vec->at(i) == max) return i;
    }

    return -1;
}

float LeafSplit::evalUtility(vector<float>* polQVals) {
    int actionChosen = vectorArgmax(polQVals);

    float leftPotUtil = vectorMax(this->leftQS) - this->leftQS->at(actionChosen);
    float rightPotUtil = vectorMax(this->rightQS) - this->rightQS->at(actionChosen);
    
    return leftPotUtil * this->leftVisits + rightPotUtil * this->rightVisits;
}
