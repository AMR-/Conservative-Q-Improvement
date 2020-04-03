#include "qtree.hpp"
#include "leafsubpolicy.hpp"

class QTreeTwo : public QTree {
    public:
        QTree* qtree;
        unordered_map<int, float>* action_map;
        vector<LeafSubpolicy*>* subpolicies;

        QTreeTwo(QTree*, unordered_map<int, float>*); // Subpolicies are generated in constructor
        void trainSubpolicies();                
};
