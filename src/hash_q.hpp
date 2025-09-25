//
// Created by jjurgenson on 25/06/20.
//

#ifndef HASH_Q_HPP
#define HASH_Q_HPP

#include "hash.hpp"
#include "counting_network.hpp"

namespace seven_jalapenos {
namespace HashQ {


class HashQ {
public:
    int const width;
    std::atomic<size_t> size; // number of segments
    CountingNetwork::CountingNetwork nq_net;
    CountingNetwork::CountingNetwork dq_net;

    HashQ(int width);
    void enqueue(int value, int id);
    int dequeue(int id);

private:
    std::unique_ptr<HashSegment> head_;
    std::atomic<HashSegment*> tail_;
    std::atomic<HashSegment*> aux_tail_;
    // aux_tail_ will catch early enqueues
};

} // HashQ
} // seven_jalapenos

#endif //HASH_Q_HPP
