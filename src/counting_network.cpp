//
// Created by jjurgenson on 25/06/19.
//

#include "counting_network.hpp"

#include <algorithm>

namespace seven_jalapenos {
namespace CountingNetwork {

size_t CountingNetwork::get_and_increment(int id) {
    ExternalBalancer<size_t[2]>* node = traverse(id);
    std::lock_guard lck(node->mtx_);
    auto arr = node->get_p();
    size_t* larger;
    size_t* smaller;
    if (arr[0] < arr[1]) {
        larger = arr[1];
        smaller = arr[0];
    }
    else {
        larger = arr[0];
        smaller = arr[1];
    }
    size_t output;
    if (*larger == 0)
        output = node->get_idx();
    else
        output = *larger + width_;
    *smaller = output;
    return output;
}

} // CountingNetwork
} // seven_jalapenos