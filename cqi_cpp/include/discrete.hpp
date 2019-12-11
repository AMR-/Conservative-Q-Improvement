#include <cstdlib>
#include <iostream>
#include "space.hpp"

class Discrete : public Space<int> {
    public:
        int n;

        Discrete(int);

        int sample();

        bool contains(int);
};
