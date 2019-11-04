#include "state.hpp"
#include "action.hpp"
#include <unordered_map>
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

        LeafSplit(int, int, vector<float>*, vector<float>*, int, int);
        ~LeafSplit();

        void update(State*, Action*, int, unordered_map<string, float>*);

        float evalUtility(vector<float>*);
}; 
