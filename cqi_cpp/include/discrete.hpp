#include "space.hpp"

class Discrete : public Space {
    public:
        int n;

        Discrete(int n);

        int sample();

        bool contains(int);

        void print();

        bool equals(Discrete);
};
