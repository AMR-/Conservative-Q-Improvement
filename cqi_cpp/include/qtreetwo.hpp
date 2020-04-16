#include "qtree.hpp"
#include "leafsubpolicy.hpp"

class QTreeTwo : public QTree {
    public:
        QTree* qtree;
        vector<LeafSubpolicy*>* subpolicies;

        QTreeTwo(QTree*); // Subpolicies are generated in constructor
        void updateSubpolicies(); // Calls update() on all LeafSubpolicies                
};
