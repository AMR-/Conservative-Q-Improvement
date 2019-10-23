#include "../../tests/env/include/state.hpp"
#include "../../tests/env/include/action.hpp"
#include "qtreeparamstore.hpp"
#include <vector>

using namespace std;

class LeafSplit {
    public:
        int feature;
        int value;

        vector<float>* leftQS; 
        vector<float>* rightQS; 
        
        int leftVisits;
        int rightVisits;

        LeafSplit(int, int, vector<float>, vector<float>, int, int);
        ~LeafSplit();

        void update(State*, Action*, int, QTreeParamStore*);

        void evalUtility(vector<float>);
}; 
