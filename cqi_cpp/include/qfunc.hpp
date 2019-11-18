#include "qtreeleaf.hpp"
#include <stdexcept>

class QFunc {
    public:
        vector<State*>* stateSpace;         
        vector<Action*>* actionSpace;         
       
        QFunc* selfCopy;
        bool makeCopies;

        QFunc(vector<State*>* stateSpace , vector<Action*>* actionSpace) {
            this->stateSpace = stateSpace;
            this->actionSpace = actionSpace;
            this->selfCopy = NULL;
            this->makeCopies = false;
        }


        virtual int selectA(State*) = 0;
        virtual void takeTuple(State*, Action*, float, State*, bool) = 0;
        virtual bool justSplit() = 0;
        
        QFunc* getPreSplit() {
            if (this->selfCopy != NULL) {
                return this->selfCopy;
            } else {
               throw std::runtime_error("selfCopy is NULL");
            }
        }

        void enableCopies() {
            this->makeCopies = true; 
        }

        void disableCopies() {
            this->makeCopies = false;
        }
};
