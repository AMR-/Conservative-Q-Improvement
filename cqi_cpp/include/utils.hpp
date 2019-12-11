#include <vector>

using namespace std;

class Utils {
    public:
        static int argmax(vector<float>* vec) {
            float max = vec->at(0);
            int argmax = 0;

            for (int i = 1; i < vec->size(); i++) {
                if (vec->at(i) > max) {
                    max = vec->at(i);
                    argmax = i;
                }
            }

            return argmax;
        }

        static vector<float>* zeros(int length) {
            vector<float>* zeros;
            zeros->reserve(length);

            for (int i = 0; i < length; i++) 
                zeros->at(i) = 0.0;

            return zeros;
        }

        static bool all(vector<float>* first, vector<float>* second, bool atLeast) {
            if (atLeast) {
                for (int i = 0; i < first->size(); i++) {
                    if (first->at(i) < second->at(i))
                        return false;
                }
            } else {
                for (i = 0; i < first->size(); i++) {
                    if (first->at(i) > second->at(i))
                        return false;
                }
            }

            return true;
        }
};
