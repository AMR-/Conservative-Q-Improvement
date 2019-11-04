#define MAX_REWARD 2147483647
#define MIN_REWARD -2147483648

#include <vector> 
#include "../../../cqi_cpp/include/action.hpp"
#include "../../../cqi_cpp/include/action_result.hpp"

using namespace std;

class Env {
    /* Environment class that serves as a template for more complex environments for CQI experiments. */

    public:
        int maxReward = MAX_REWARD; 
        int minReward = MIN_REWARD;

        vector<Action*>* actionSpace;
        vector<State*>* stateSpace;

        Env();
        ~Env();

        ActionResult step(Action);
        State reset();
        void close();
};

/*  TODO: write sample env class in C++, indicate its location
          write implementation of OpenAI Gym env
          write tests */
