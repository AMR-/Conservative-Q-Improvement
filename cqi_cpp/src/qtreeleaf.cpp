#include "../include/qtreenode.hpp"
#include "../include/utils.hpp"
#include <iostream>
#include <vector>

class QTreeLeaf : public QTreeNode { 
    public:
        vector<float>* qs;
        vector<LeafSplit*>* splits;  

        QTreeLeaf(vector<float>* qs, float visits, vector<LeafSplit*>* splits) : QTreeNode(visits) {
            this->qs = qs;
            this->splits = splits;
        }

        ~QTreeLeaf();

        bool isLeaf() {
            return true;
        }

        vector<float>* getQS() {
            return this->qs;
        }
        
        void update(State* s, Action* a, int target, unordered_map<string, float>* params) {
            QTreeNode::update(s, a, target, params);
            this->qs->at(a->value) = (1 - params->at("alpha")) * this->qs->at(a->value) + params->at("alpha") * target;

            for (auto& split : *(this->splits))
                split->update(s, a, target, params);
        }
        
        QTreeNode* split(State* s, vector<int>* boxLow, vector<int>* boxHigh, unordered_map<string, float>* params) {
            vector<float> splitUtils;

            for (auto& sp: *(this->splits))
                splitUtils.push_back(sp->evalUtility(this->qs));

            int splitIndex = Utils::vectorArgmax(&splitUtils);

            LeafSplit* sfSplit = this->splits->at(splitIndex);
            int splitFeature = sfSplit->feature;
            vector<LeafSplit*>* LSplits; 
            vector<LeafSplit*>* RSplits;

            for (auto& sp: *(this->splits)) {
                if (sp->feature != splitFeature) {
                    LSplits->push_back(new LeafSplit(sp->feature, sp->value, sfSplit->leftQS, sfSplit->leftQS, 0.5, 0.5));
                    RSplits->push_back(new LeafSplit(sp->feature, sp->value, sfSplit->rightQS, sfSplit->rightQS, 0.5, 0.5));
                }
            }

            for (int i = 0; i < params->at("numSplits"); i++) {
                LSplits->push_back(new LeafSplit(splitFeature, boxLow->at(splitFeature) + (sfSplit->value - boxLow->at(splitFeature))/(params->at("numSplits")+1)*(i+1), 
                    sfSplit->leftQS, sfSplit->leftQS, 0.5, 0.5));
                RSplits->push_back(new LeafSplit(splitFeature, sfSplit->value + (boxHigh->at(splitFeature) - sfSplit->value)/(params->at("numSplits")+1)*(i+1), 
                    sfSplit->rightQS, sfSplit->rightQS, 0.5, 0.5));
            }

            QTreeLeaf* leftChild = new QTreeLeaf(sfSplit->leftQS,  sfSplit->leftVisits, LSplits);
            QTreeLeaf* rightChild = new QTreeLeaf(sfSplit->rightQS, sfSplit->rightVisits, RSplits);
            
            float val = (boxHigh->at(splitFeature) + boxLow->at(splitFeature)) / 2;
            int visits = this->visits;
        
            // TODO: Resolve dependency on QTreeInternal class
            return new QTreeInternal(leftChild, rightChild, splitFeature, val, visits);
        }

        float maxSplitUntil() {
            vector<float> evalUtilities;

            for (auto& sp: *(this->splits))
                evalUtilities.push_back(sp->evalUtility(this->qs));

            return this->visits * Utils::vectorMax(&evalUtilities);
        }

        int numNodes() {
            return 1; 
        }

        void printStructure(string* prefixHead, string* prefixTail) {
            printf("%s (vis: 1.2%f) qvals: %s\n", prefixHead, this->qs);
        }
};
