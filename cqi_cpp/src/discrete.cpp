#include "../include/discrete.hpp"

Discrete::Discrete(int n) : Space(make_tuple(n, 1)) {
    this->n = n; 
}

int Discrete::sample() {
    return rand() % this->n;
}

bool Discrete::contains(int elem) {
    return elem >= 0 and elem <= this->n - 1;
}
