#include "../../tests/env/include/state.hpp"
#include "../../tests/env/include/action.hpp"
#include "qtreeparamstore.hpp"
#include "../include/leafsplit.hpp"
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

        void update(State* s, Action* a, int target, QTreeParamStore* params) {
            this->visits = this->visits * params->val("visitDecay") + (1 - params->val("visitDecay"));
        }

        void noVisitUpdate(QTreeParamStore* params) {
            this->visits = this->visits * params->val("visitDecay");
        }

        void split(State*, vector<int>*, vector<int>*, QTreeParamStore*);

        void maxSplitUntil(State*);

        int numNodes();

        void printStructure(string*, string*);
};
