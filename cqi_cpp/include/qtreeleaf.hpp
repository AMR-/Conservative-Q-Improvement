#include "qtreeinternal.hpp"

class QTreeLeaf : public QTreeNode { 
    public:
        vector<double>* qs;
        vector<vector<double>*>* qvs;
        vector<LeafSplit*>* splits;  

        QTreeLeaf(vector<double>*, vector<vector<double>*>*, double, vector<LeafSplit*>*); 
        bool isLeaf();
        vector<double>* getQS(State*); 
        vector<double>* getQS(State*, int index); 
        void update(State*, Action*, double, unordered_map<string, double>*); 
        void update(State*, Action*, double, unordered_map<string, double>*, int index); 
        QTreeInternal* split(State*, vector<double>*, vector<double>*, unordered_map<string, double>*); 
        void noVisitUpdate(unordered_map<string, double>*);
        double maxSplitUtil(State* s); 
        double maxSplitUtil(State* s, int index); 
        int numNodes(); 
        void printStructure(string, string); 
};
