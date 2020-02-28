#include "qfunc.hpp"

class QTree: public QFunc {
    public:
        double splitThreshMax, splitThreshDecay, splitThresh; 
        QTreeNode* root;
        bool _justSplit;
        unordered_map<string, double>* params;

        QTree(Box*, Discrete*, QTreeNode*, double, double, double, double, double, int);
        int selectA(State*);
        void takeTuple(State*, Action*, double, State*, bool);
        void update(State*, Action*, double, State*, bool);
        int numNodes();
        void printStructure();
        bool justSplit();
};
