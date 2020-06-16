#include "qfunc.hpp"

class QTree: public QFunc {
    public:
        double splitThreshMax, splitThreshDecay, splitThresh; 
        QTreeNode* root;
        bool _justSplit;
        bool mql;
        unordered_map<string, double>* params;
        int c;
        int timesteps;
        vector<double>* frequencies;

        QTree(Box*, Discrete*, QTreeNode*, double, double, double, double, double, int, int, int);
        int selectA(State*);
        int selectAWithUCB(State*);
        int selectAWithMQL(State*);
        void takeTuple(State*, Action*, double, State*, bool);
        void update(State*, Action*, double, State*, bool);
        void updateA(State*, Action*, double, State*, bool);
        void updateB(State*, Action*, double, State*, bool);
        void updateC(State*, Action*, double, State*, bool);
        int numNodes();
        void printStructure();
        bool justSplit();
        void incrementVals(Action*);
};
