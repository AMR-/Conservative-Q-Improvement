#include <vector>

using namespace std;

class Utils {
    public:
        static int vectorArgmax(vector<float>* vec) {
            float max = vec->at(0);
            int argmax = 0;

            for (int i = 1; i < (int) vec->size(); i++) {
                if (vec->at(i) > max) {
                    max = vec->at(i);
                    argmax = i;
                }
            }

            return argmax;
        }

        static vector<float>* zeros(int length) {
            vector<float>* zeros;

            for (int i = 0; i < length; i++) {
                zeros->push_back(0.0);
            }

            return zeros;
        }
};
