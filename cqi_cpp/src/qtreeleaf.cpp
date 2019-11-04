#include "../include/qtreenode.hpp"

class QTreeLeaf : public QTreeNode { 
    public:
        vector<float>* qs;
        vector<LeafSplit*>* splits;  

        typedef QTreeNode super;

        QTreeLeaf() : super(visits) {
            // TODO
        }

        ~QTreeLeaf();

        bool isLeaf() {
            // TODO
        }

        vector<float> getQS(State* s) {
            // TODO
        }
        
        void update(State* s, Action* a, int target, unordered_map<string, float>* params) {
            // TODO
        }

        void split(State* s, vector<int>* boxLow, vector<int>* boxHigh, unordered_map<string, float>* params) {
            // TODO
        }

        void maxSplitUntil(State* s) {
            // TODO
        }

        int numNodes() {
            // TODO
        }

        void printStructure(string* prefixHead, string* prefixTail) {
            // TODO
        }
};

