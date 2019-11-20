#include "qtreenode.hpp"
#include <iostream>
#include <utility> 

class QTreeInternal: public QTreeNode { 
    public:
        QTreeNode* leftChild;
        QTreeNode* rightChild;
        int feature;
        float value;
        float visits;
       
        QTreeInternal(QTreeNode* leftChild, QTreeNode* rightChild, int feature, float value, float visits); 

        ~QTreeInternal();

        bool isLeaf();

        vector<float>* getQS(State* s); 

        void update(State* s, Action* a, int target, unordered_map<string, float>* params); 

        QTreeInternal* split(State* s, vector<float>* boxLow, vector<float>* boxHigh, unordered_map<string, float>* params); 

        pair<QTreeNode*, QTreeNode*> selectChild(State* s); 

        float maxSplitUntil(State* s); 

        int numNodes(); 

        void printStructure(string prefixHead, string prefixTail); 
};
