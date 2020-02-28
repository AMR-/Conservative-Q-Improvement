#include "qtreeinternal.hpp"

class QTreeLeaf : public QTreeNode { 
    public:
        vector<double>* qs;
        vector<LeafSplit*>* splits;  

        QTreeLeaf(vector<double>*, double, vector<LeafSplit*>*);
        
        ~QTreeLeaf();
        
        bool isLeaf();

        vector<double>* getQS(State*); 
        
        void update(State*, Action*, double, unordered_map<string, double>*); 
        
        QTreeInternal* split(State*, vector<double>*, vector<double>*, unordered_map<string, double>*); 

        void noVisitUpdate(unordered_map<string, double>*);
        
        double maxSplitUtil(State* s); 

        int numNodes(); 

        void printStructure(string, string); 
};
