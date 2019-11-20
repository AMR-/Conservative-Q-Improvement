#include "qtreeinternal.hpp"

class QTreeLeaf : public QTreeNode { 
    public:
        vector<float>* qs;
        vector<LeafSplit*>* splits;  

        QTreeLeaf(vector<float>* qs, float visits, vector<LeafSplit*>* splits);
        
        ~QTreeLeaf();
        
        bool isLeaf();

        vector<float>* getQS(State* s); 
        
        void update(State* s, Action* a, int target, unordered_map<string, float>* params); 
        
        QTreeInternal* split(State* s, vector<float>* boxLow, vector<float>* boxHigh, unordered_map<string, float>* params); 

        float maxSplitUntil(State* s); 

        int numNodes(); 

        void printStructure(string prefixHead, string prefixTail); 
};
