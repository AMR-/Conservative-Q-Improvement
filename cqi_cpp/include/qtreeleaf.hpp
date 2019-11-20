#include "qtreeinternal.hpp"

class QTreeLeaf : public QTreeNode { 
    public:
        vector<float>* qs;
        vector<LeafSplit*>* splits;  

        QTreeLeaf(vector<float>*, float, vector<LeafSplit*>*);
        
        ~QTreeLeaf();
        
        bool isLeaf();

        vector<float>* getQS(State*); 
        
        void update(State*, Action*, int, unordered_map<string, float>*); 
        
        QTreeInternal* split(State*, vector<float>*, vector<float>*, unordered_map<string, float>*); 

        float maxSplitUntil(State* s); 

        int numNodes(); 

        void printStructure(string, string); 
};
