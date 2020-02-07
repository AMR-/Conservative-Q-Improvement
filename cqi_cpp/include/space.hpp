#ifndef SPACE_H
#define SPACE_H
#include <tuple>
#include <vector>

using namespace std; 

template<typename T>
class Space {
    public:
        tuple<int, int>* shape;

		Space<T>(tuple<int, int>* shape) {
    		this->shape = shape;
		}

        virtual T sample() = 0;

        virtual bool contains(T) = 0;
};
#endif
