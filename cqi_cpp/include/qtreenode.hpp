#include "../include/leafsplit.hpp"
#include <string>

class QTreeNode {
    public:
        float visits; 
        
        QTreeNode(float visits) {
            this->visits = visits;
        }

        ~QTreeNode();

        virtual bool isLeaf() = 0;
        
        virtual vector<float>* getQS(State*) = 0;

        void update(State* s, Action* a, int target, unordered_map<string, float>* params) {
            this->visits = this->visits * params->at("visitDecay") + (1 - params->at("visitDecay"));
        }

        void noVisitUpdate(unordered_map<string, float>* params) {
            this->visits = this->visits * params->at("visitDecay");
        }

        virtual QTreeNode* split(State*, vector<float>*, vector<float>*, unordered_map<string, float>*) = 0;
        
        virtual float maxSplitUntil(State*) = 0;

        virtual int numNodes() = 0;

        virtual void printStructure(string, string) = 0;
};
