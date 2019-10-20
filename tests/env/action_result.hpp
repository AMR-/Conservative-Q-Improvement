#include "state.hpp"

class ActionResult {
    public:
        State* observation;
        float reward;
        bool done;

        ActionResult(State*, float, bool);
        ~ActionResult();
};
