//
// Created by jjurgenson on 25/06/19.
//

#ifndef COUNTING_NETWORK_HPP
#define COUNTING_NETWORK_HPP
#include "balancing_network.hpp"

namespace seven_jalapenos {
namespace CountingNetwork {

class CountingNetwork : BalancingNetwork {
public:
    // std::vector<size_t> counts;
    // std::vector<std::mutex> mtx_arr_;
    std::vector<std::atomic<size_t>> counts;

    explicit CountingNetwork(int width);

    std::tuple<size_t, size_t> get_and_increment(int id);

    [[nodiscard]] int width() const override;
};

} // CountingNetwork
} // seven_jalapenos

#endif //COUNTING_NETWORK_HPP
