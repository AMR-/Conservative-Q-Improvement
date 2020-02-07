#include "../include/qtree.hpp"

int main() {
	vector<float>* low = new vector<float>();
	low->push_back(1.0);
	low->push_back(5.0);

	vector<float>* high = new vector<float>();
	high->push_back(3.0);
	high->push_back(6.0);

	// sample \in [1.0, 3.0] \times [5.0, 6.0]

	Box* box = new Box(low, high);

	vector<float>* res = new vector<float>();

	res->push_back(1.0);
	res->push_back(5.0);

	cout << box->contains(res)  << endl;

	return 0;
}
