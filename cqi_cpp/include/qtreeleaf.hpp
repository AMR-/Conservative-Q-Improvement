#include "qtreeinternal.hpp"

class QTreeLeaf : public QTreeNode { 
    public:
        vector<double>* qs;
        vector<double>* qas;
        vector<double>* qbs;
        vector<LeafSplit*>* splits;  

        QTreeLeaf(vector<double>*, vector<double>*, vector<double>*, double, vector<LeafSplit*>*);
        bool isLeaf();
        vector<double>* getQS(State*); 
        vector<double>* getQAS(State*); 
        vector<double>* getQBS(State*); 
        void update(State*, Action*, double, unordered_map<string, double>*); 
        void updateA(State*, Action*, double, unordered_map<string, double>*); 
        void updateB(State*, Action*, double, unordered_map<string, double>*); 
        QTreeInternal* split(State*, vector<double>*, 
                        vector<double>*, unordered_map<string, double>*); 
        void noVisitUpdate(unordered_map<string, double>*);
        double maxSplitUtil(State* s); 
        double maxSplitUtilA(State* s); 
        double maxSplitUtilB(State* s); 
        int numNodes(); 
        void printStructure(string, string); 
};
