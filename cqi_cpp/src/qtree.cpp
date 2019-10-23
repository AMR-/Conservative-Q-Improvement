#include "../include/qfunc.hpp"

class QTree: public QFunc {
    public:
        typedef QFunc super;

        float gamma, alpha, visitDecay, splitThreshMax, splitThreshDecay;

        int numSplits;

        vector<State*> stateSpace;
        vector<Action*> actionSpace;

        QTree() : super(stateSpace, actionSpace) {
            // TODO
        }

        float selectA(State* s) {
            // TODO
        }

        void takeTuple(State* s, Action* a, float r, State* s2, bool done) {
            // TODO
        }

        void update(State* s, Action* a, float r, State* s2, bool done) {
            // TODO
        }

        int numNodes() {
            // TODO
        }

        void printStructure() {
            // TODO
        }
        
        bool justSplit() {
            // TODO
        }
};
