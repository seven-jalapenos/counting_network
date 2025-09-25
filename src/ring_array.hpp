
#ifndef RING_ARRAY_HPP
#define RING_ARRAY_HPP

#include <vector>
#include "hash.hpp"

namespace seven_jalapenos::TTQ { // Tile Ticket Queue

class RingArray {
public:
    const size_t length;
    std::vector<seven_jalapenos::HashQ::Hash::HashCell> arr;

    RingArray(size_t length);
    size_t get_idx(size_t idx) const;
};


} //seven_jalapenos::TTQ



#endif // RING_ARRAY_HPP