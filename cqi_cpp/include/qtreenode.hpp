#ifndef QTREENODE_H
#define QTREENODE_H

#include "../include/leafsplit.hpp"
#include <string>

class QTreeNode {
    public:
        double visits; 
        
        QTreeNode(double visits) {
            this->visits = visits;
        }

        virtual bool isLeaf() = 0;
        virtual vector<double>* getQS(State*) = 0;
        virtual vector<double>* getQS(State*, int) = 0;
        
        virtual void update(State*, Action*, double, unordered_map<string, double>*) = 0;
        virtual void update(State*, Action*, double, unordered_map<string, double>*, int) = 0;

        virtual void noVisitUpdate(unordered_map<string, double>*) = 0;
        virtual QTreeNode* split(State*, vector<double>*, vector<double>*, unordered_map<string, double>*) = 0;
        virtual double maxSplitUtil(State*) = 0;
        virtual double maxSplitUtil(State*, int) = 0;
        virtual int numNodes() = 0;
        virtual void printStructure(string, string) = 0;
};
#endif
