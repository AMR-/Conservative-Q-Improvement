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
       
        QTreeInternal(QTreeNode*, QTreeNode*, int, float, float); 

        ~QTreeInternal();

        bool isLeaf();

        vector<float>* getQS(State*); 

        void update(State*, Action*, int, unordered_map<string, float>*); 

        QTreeInternal* split(State*, vector<float>*, vector<float>*, unordered_map<string, float>*); 

        pair<QTreeNode*, QTreeNode*> selectChild(State*); 

        float maxSplitUntil(State*); 

        int numNodes(); 

        void printStructure(string, string); 
};
