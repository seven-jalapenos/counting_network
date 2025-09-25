
#include "ring_array.hpp"
#include <cstddef>


namespace seven_jalapenos::TTQ { 

RingArray::RingArray(size_t length): length(length), arr(length){}

size_t RingArray::get_idx(size_t idx) const { return idx % length; }

} //seven_jalapenos::TTQ