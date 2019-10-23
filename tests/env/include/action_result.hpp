#include "state.hpp"

class ActionResult {
    public:
        State* state;
        float reward;
        bool done;

        ActionResult(State*, float, bool);
        ~ActionResult();
};
