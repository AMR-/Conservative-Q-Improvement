#ifndef UTILS_H
#define UTILS_H
#include <vector>
#include <sstream>
using namespace std;

class Utils {
    public:
        static int argmax(vector<float>* vec) {
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
            vector<float>* zeros = new vector<float>();

            for (int i = 0; i < length; i++) 
                zeros->push_back(0);

            return zeros;
        }

        static bool all(vector<float>* first, vector<float>* second, bool atLeast) {
            if (atLeast) {
                for (size_t i = 0; i < first->size(); i++) {
                    if (first->at(i) < second->at(i))
                        return false;
                }
            } else {
                for (size_t i = 0; i <  first->size(); i++) {
                    if (first->at(i) > second->at(i))
                        return false;
                }
            }

            return true;
        }

		static string vecToString(vector<float>* vec) {
			stringstream ss;

			ss << "[";
			for (size_t i = 0; i < vec->size(); i++) {
				if (i > 0)
					ss << ", ";

				ss << vec->at(i);
			}
			ss << "]";

			string res = ss.str();
			
			return res;
		}
};
#endif
