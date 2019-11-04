#include "leafsplit.hpp"
#include <string>

using namespace std;

class QTreeNode {
    public:
        int visits; 
        
        QTreeNode(int visits) {
            this->visits = visits;
        }

        ~QTreeNode();

        bool isLeaf();
        vector<float> getQS(State*);

        void update(State* s, Action* a, int target, unordered_map<string, float>* params) {
            this->visits = this->visits * params->at("visitDecay") + (1 - params->at("visitDecay"));
        }

        void noVisitUpdate(unordered_map<string, float>* params) {
            this->visits = this->visits * params->at("visitDecay");
        }

        void split(State*, vector<int>*, vector<int>*, unordered_map<string, float>*);

        void maxSplitUntil(State*);

        int numNodes();

        void printStructure(string*, string*);
};
