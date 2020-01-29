#include "space.hpp"
#include "utils.hpp"

class Box : public Space<vector<float>*> {
    public:
        vector<float>* low;
        vector<float>* high;

        Box(vector<float>*, vector<float>*, tuple<int, int>);

        vector<float>* sample();
        
        bool contains(vector<float>*);
};
