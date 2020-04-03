#include "action.h"

class LeafSubpolicy {
    public:
        Action* action;

        LeafSubpolicy(Action*);
        void train(); // train using multi-parameter linear regression 
};
