#include "../include/qtreenode.hpp"
#include <utility> 

using namespace std;

class QTreeInternal: public QTreeNode {
    public:
        QTreeNode* leftChild, rightChild;
        int feature, value;
       
        typedef QTreeNode super;

        QTreeInternal::QTreeInternal() {
            // TODO
        }

        ~QTreeInternal();

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

        pair<QTree*, QTree*>* selectChild() {
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
