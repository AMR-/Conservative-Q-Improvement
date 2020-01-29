#include "space.hpp"
#include "action.hpp"
#include <cstdlib>
#include <iostream>

class Discrete : public Space<int> {
    public:
        int n;

        Discrete(int);

        int sample();

        int size();

        bool contains(int);
};
