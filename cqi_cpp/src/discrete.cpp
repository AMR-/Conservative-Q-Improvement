#include "../include/discrete.hpp"

Discrete::Discrete(int n) : Space(new tuple<int, int>(n, 1)) {
    this->n = n; 
}

int Discrete::sample() {
    return rand() % this->n;
}

int Discrete::size() {
    return this->n;
}

bool Discrete::contains(int elem) {
    return elem >= 0 and elem <= this->n - 1;
}
