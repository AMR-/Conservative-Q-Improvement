#include "qtreeleaf.hpp"
#include "box.hpp"
#include "discrete.hpp"
#include <stdexcept>
#include <math.h>

class QFunc {
    public:
        Box* stateSpace;         
        Discrete* actionSpace;         
       
        QFunc* selfCopy;
        bool makeCopies;

        QFunc(Box* stateSpace , Discrete* actionSpace) {
            this->stateSpace = stateSpace;
            this->actionSpace = actionSpace;
            this->selfCopy = nullptr;
            this->makeCopies = false;
        }

        virtual int selectA(State*) = 0;
        virtual int selectAWithUCB(State*) = 0;
        virtual void takeTuple(State*, Action*, double, State*, bool) = 0;
        virtual bool justSplit() = 0;
        virtual void incrementVals(Action*) = 0;

        QFunc* getPreSplit() {
            if (this->selfCopy != nullptr) {
                return this->selfCopy;
            } else {
               throw runtime_error("selfCopy is NULL");
            }
        }

        void enableCopies() {
            this->makeCopies = true; 
        }

        void disableCopies() {
            this->makeCopies = false;
        }
};
