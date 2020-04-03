#include <unordered_map>

class QTreeWrapper {
    public:
        // Mapping of discrete actions to representative actions
        unordered_map<int, double>* action_map;

        QTreeWrapper(unordered_map<int, double>*); // Reads mapping from file
        void put(int, double);
        double get(double);
};
