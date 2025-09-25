//
// Created by jjurgenson on 25/06/19.
//

#include "counting_network.hpp"
#include <atomic>
#include <cstddef>

// #include <algorithm>

namespace seven_jalapenos {
namespace CountingNetwork {

CountingNetwork::CountingNetwork(int width)
    :BalancingNetwork(width), counts(width) {
    for (size_t i = 0; i < width; i++) {
        counts[i].store(i);
    }
}

std::tuple<size_t, size_t> CountingNetwork::get_and_increment(int id) {
    using std::array;
    auto [order, count] = traverse(id);
    size_t last = counts[order].fetch_add(width_, std::memory_order_relaxed);
    size_t epoch = last / width_;
    return {last, epoch};
}

[[nodiscard]] int CountingNetwork::width() const { return width_; }

} // CountingNetwork
} // seven_jalapenos