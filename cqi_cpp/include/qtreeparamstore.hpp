#include <unordered_map>
#include <string>

using namespace std;

class QTreeParamStore {
    public:
        unordered_map<string, float> paramMap;

        QTreeParamStore(unordered_map<string, float>);
        ~QTreeParamStore();

        void setParam(string, float);

        float val(string); 
};
