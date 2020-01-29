#include "../include/qtree.hpp"

int main() {
	vector<float>* vec = new vector<float>();

	for (int i = 0; i < 4; i++)
		vec->push_back(1.5);
	
	string s = Utils::vecToString(vec);

	printf("%s\n", s.c_str());

	return 0;
}
