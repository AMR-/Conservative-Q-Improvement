#include "../../tests/env/include/state.hpp"
#include "../../tests/env/include/action.hpp"

class QFunc {
    public:
        vector<State*> stateSpace;         
        vector<Action*> actionSpace;         
       
        QFunc* selfCopy;
        bool makeCopies;

        QFunc(vector<State*> stateSpace , vector<Action*> actionSpace) {
            this->stateSpace = stateSpace;
            this->actionSpace = actionSpace;
            this->selfCopy = nullptr;
            this->makeCopies = false;
        }

        ~QFunc();

        float selectA(State*);
        void takeTuple(State*, Action*, float, State*, bool);
        bool justSplit();
        
        QFunc* getPreSplit();

        void enableCopies() {
            this->makeCopies = true; 
        }

        void disableCopies() {
            this->makeCopies = false;
        }
};
