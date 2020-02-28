#include "../include/qtree.hpp"

int main() {
	vector<double>* low = new vector<double>();
	low->push_back(1.0);
	low->push_back(5.0);

	vector<double>* high = new vector<double>();
	high->push_back(3.0);
	high->push_back(6.0);

	// sample \in [1.0, 3.0] \times [5.0, 6.0]

	Box* box = new Box(low, high);

	vector<double>* res = new vector<double>();

	res->push_back(1.0);
	res->push_back(5.0);

	cout << box->contains(res)  << endl;

	return 0;
}
