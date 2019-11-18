#include "state.hpp"
#include "action.hpp"
#include "utils.hpp"
#include <unordered_map>
#include <algorithm>

class LeafSplit {
    public:
        int feature;
        float value;

        vector<float>* leftQS; 
        vector<float>* rightQS; 
        
        float leftVisits;
        float rightVisits;

        LeafSplit(int, float, vector<float>*, vector<float>*, float, float);
        ~LeafSplit();

        void update(State*, Action*, int, unordered_map<string, float>*);

        float evalUtility(vector<float>*);
}; 
