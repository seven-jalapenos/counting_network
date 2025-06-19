//
// Created by jjurgenson on 25/06/19.
//

#ifndef COUNTING_NETWORK_HPP
#define COUNTING_NETWORK_HPP
#include "ordering_network.hpp"

namespace seven_jalapenos {
namespace CountingNetwork {

// class CountingTuple {
// public:
//     size_t arr[];
//
// };

class CountingNetwork : OrderingNetwork<size_t[]> {
public:
    explicit CountingNetwork(int width)
    : OrderingNetwork(width) {}

    size_t get_and_increment(int id);
};

} // CountingNetwork
} // seven_jalapenos

#endif //COUNTING_NETWORK_HPP
