#ifndef UTILS_H
#define UTILS_H
#include <vector>
#include <sstream>
using namespace std;

class Utils {
    public:
        static int argmax(vector<double>* vec) {
            double vmax = vec->at(0);
            int argmax = 0;

            for (size_t i = 1; i < vec->size(); i++) {
                if (vec->at(i) > vmax) {
                    vmax = vec->at(i);
                    argmax = i;
                }
            }

            return argmax;
        }
        
        static double max(vector<double>* vec) {
            double vmax = vec->at(0);

            for (size_t i = 1; i < vec->size(); i++)
                if (vec->at(i) > vmax) vmax = vec->at(i);

            return vmax;
        }

        static vector<double>* zeros(int length) {
            vector<double>* zeros = new vector<double>();

            for (int i = 0; i < length; i++) 
                zeros->push_back(0);

            return zeros;
        }

        static vector<double>* copy(vector<double>* vec) {
            vector<double>* vcopy = new vector<double>();

            for (size_t i = 0; i < vec->size(); i++)
                vcopy->push_back(vec->at(i));
           
            return vcopy;
        }

        static bool all(vector<double>* first, vector<double>* second, bool atLeast) {
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

		static string vecToString(vector<double>* vec) {
			stringstream ss;

			ss << "[";
			for (size_t i = 0; i < vec->size(); i++) {
				if (i > 0) ss << ", ";

				ss << vec->at(i);
			}
			ss << "]";

			string res = ss.str();
			
			return res;
		}
};
#endif
