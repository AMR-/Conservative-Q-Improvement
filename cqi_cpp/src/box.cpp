#include "../include/box.hpp"

Box::Box(vector<float> low, vector<float> high, tuple<int, int> shape) : Space(shape) {
    this->low = low;
    this->high = high;
}

// TODO
vector<float> Box::sample() {
    

    /* Python code
    sample = np.empty(self.shape)

    // Masking arrays which classify the coordinates according to interval
    // type
    unbounded   = ~self.bounded_below & ~self.bounded_above
    upp_bounded = ~self.bounded_below &  self.bounded_above
    low_bounded =  self.bounded_below & ~self.bounded_above
    bounded     =  self.bounded_below &  self.bounded_above

    // Vectorized sampling by interval type
    sample[unbounded] = self.np_random.normal(size=unbounded[unbounded].shape)

    sample[low_bounded] = self.np_random.exponential(size=low_bounded[low_bounded].shape) + self.low[low_bounded]

    sample[upp_bounded] = -self.np_random.exponential(size=upp_bounded[upp_bounded].shape) - self.high[upp_bounded]

    sample[bounded] = self.np_random.uniform(low=self.low[bounded],high=high[bounded], size=bounded[bounded].shape)
    
    if self.dtype.kind == 'i':
        sample = np.floor(sample)

    return sample.astype(self.dtype)
    */
}

bool Box::contains(vector<float> x) {
    return Utils::all(x, this->low, true) and Utils::all(x, this->high, false); 
}
