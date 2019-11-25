#include "utils.hpp"
#include "space.hpp"

// TODO
class Box : public Space {
    public:
        ? low;
        ? high;

        Box(?, ?, ?);

        bool isBounded(string);

        ? sample();
        
        bool contains(?);

        void print();

        bool equals(Box);
};
