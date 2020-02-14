#include "../include/box.hpp"

Box::Box(vector<float>* low, vector<float>* high) : Space(new tuple<int, int>(low->size(), high->size())) {
    this->low = low;
    this->high = high;
}

vector<float>* Box::sample() {
	vector<float>* sample = new vector<float>();
	random_device rd;
	mt19937 gen(rd()); 

	for (size_t i = 0; i < this->low->size(); i++) {
		uniform_real_distribution<> dis(this->low->at(i), this->high->at(i)); 

		sample->push_back(dis(gen));
	}

    return sample;
}

bool Box::contains(vector<float>* x) {
    return Utils::all(x, this->low, true) and Utils::all(x, this->high, false);
}

