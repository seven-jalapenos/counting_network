//
// Created by jjurgenson on 25/06/19.
//

#include "counting_network.hpp"

#include <array>

namespace seven_jalapenos {
namespace CountingNetwork {

CountingNetwork::CountingNetwork(int width)
    :OrderingNetwork(width), counts(width), mtx_arr_(width) {
        for (int i = 0; i < width; i++) {
            counts[i].count[0] = i;
        }
    }
size_t CountingNetwork::get_and_increment(int id) {
    using std::array;
    int order = traverse(id);
    std::lock_guard lck(mtx_arr_[order]);
    CountingTuple & curr = std::ref(counts[order]);
    size_t output;
    if (curr.idx0_is_larger) {
        output = curr.count[0] + width_;
        curr.count[1] = output;
        curr.idx0_is_larger = false;
    }
    else {
        output = curr.count[1] + width_;
        curr.count[0] = output;
        curr.idx0_is_larger = true;
    }
    return output;
}

} // CountingNetwork
} // seven_jalapenos