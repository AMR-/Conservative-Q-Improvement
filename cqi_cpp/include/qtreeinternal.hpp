#include "qtreenode.hpp"
#include <iostream>
#include <tuple> 

class QTreeInternal: public QTreeNode { 
    public:
        QTreeNode* leftChild;
        QTreeNode* rightChild;
        int feature;
        double value;
        double visits;
       
        QTreeInternal(QTreeNode*, QTreeNode*, int, double, double); 
        bool isLeaf();
        vector<double>* getQS(State*); 
        vector<double>* getQAS(State*); 
        vector<double>* getQBS(State*); 
        vector<double>* getQCS(State*); 
        void update(State*, Action*, double, unordered_map<string, double>*); 
        void updateA(State*, Action*, double, unordered_map<string, double>*); 
        void updateB(State*, Action*, double, unordered_map<string, double>*); 
        void updateC(State*, Action*, double, unordered_map<string, double>*); 
        QTreeInternal* split(State*, vector<double>*, vector<double>*, 
                        unordered_map<string, double>*); 
        void noVisitUpdate(unordered_map<string, double>*);
        tuple<QTreeNode*, QTreeNode*> selectChild(State*); 
        double maxSplitUtil(State*); 
        double maxSplitUtilA(State*); 
        double maxSplitUtilB(State*); 
        double maxSplitUtilC(State*); 
        int numNodes(); 
        void printStructure(string, string); 
};
