//
// Created by jjurgenson on 25/06/19.
//

#include "counting_network.hpp"

#include <array>

namespace seven_jalapenos {
namespace CountingNetwork {

CountingNetwork::CountingNetwork(int width)
    :BalancingNetwork(width), counts(width), mtx_arr_(width) {
        for (size_t i = 0; i < width; i++) {
            counts[i] = i;
        }
    }

size_t CountingNetwork::get_and_increment(int id) {
    using std::array;
    int order = traverse(id);
    std::lock_guard lck(mtx_arr_[order]);
    size_t last = counts[order];
    counts[order] += width_;
    return last;
}

} // CountingNetwork
} // seven_jalapenos