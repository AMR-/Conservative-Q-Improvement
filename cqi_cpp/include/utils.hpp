#include <vector>

using namespace std;

class Utils {
    public:
        static float vectorMax(vector<float>* vec) {
            float max = vec->at(0);

            for (auto& elem: *vec) {
                if (elem > max) max = elem;
            }   

            return max;
        }

        static int vectorArgmax(vector<float>* vec) {
            float max = vectorMax(vec);

            for (int i = 0; i < (int) vec->size(); i++) {
                if (vec->at(i) == max) return i;
            }

            return -1;
        }
};
