#include <unordered_map>

class QTreeWrapper {
    public:
        // Mapping of discrete actions to representative actions
        unordered_map<int, float>* action_map;

        QTreeWrapper(unordered_map<int, float>*); // Reads mapping from file
        void put(int, float);
        float get(float);
};
