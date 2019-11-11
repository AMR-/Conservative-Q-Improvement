#include "../include/leafsplit.hpp"
#include <string>

using namespace std;

class QTreeNode {
    public:
        float visits; 
        
        QTreeNode(float visits) {
            this->visits = visits;
        }

        ~QTreeNode();

        virtual bool isLeaf();
        
        virtual vector<float>* getQS(State* s);

        void update(State* s, Action* a, int target, unordered_map<string, float>* params) {
            this->visits = this->visits * params->at("visitDecay") + (1 - params->at("visitDecay"));
        }

        void noVisitUpdate(unordered_map<string, float>* params) {
            this->visits = this->visits * params->at("visitDecay");
        }

        virtual QTreeNode* split(State*, vector<int>*, vector<int>*, unordered_map<string, float>*);
        
        virtual float maxSplitUntil(State*);

        virtual int numNodes();

        virtual void printStructure(string*, string*);
};
