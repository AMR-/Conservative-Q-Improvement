#include "../include/qfunc.hpp"

class QTree: public QFunc {
    public:
        float splitThreshMax, splitThreshDecay, splitThresh; 

        QTreeNode* root;

        bool _justSplit;

        unordered_map<string, float>* params;

        QTree(vector<State*>*, vector<Action*>*, QTreeNode*, float, float, float, float, 
            float, int);
       
        QTree(const QTree &obj);

        ~QTree();

        vector<float>* getLow(vector<State*>*);

        vector<float>* getHigh(vector<State*>*);

        int selectA(State*);

        void takeTuple(State*, Action*, float, State*, bool);
        
        void update(State*, Action*, float, State*, bool);

        int numNodes();

        void printStructure();
        
        bool justSplit();
};
