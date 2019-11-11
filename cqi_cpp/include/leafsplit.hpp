#include "state.hpp"
#include "action.hpp"
#include <unordered_map>
#include <vector>

using namespace std;

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
