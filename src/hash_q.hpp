//
// Created by jjurgenson on 25/06/20.
//

#ifndef HASH_Q_HPP
#define HASH_Q_HPP

#import "hash.hpp"
#import "counting_network.hpp"

namespace seven_jalapenos {
namespace HashQ {


class HashQ {
public:
    int const net_width;
    int const hash_length;
    size_t q_length;
    CountingNetwork::CountingNetwork nq_net;
    CountingNetwork::CountingNetwork dq_net;

    HashQ(int length, int width)
        :net_width(width), hash_length(length), q_length(1),
         nq_net(width), dq_net(width),
         head_(new HashSegment(length)), tail_(head_.get()), aux_tail_(new HashSegment(length)) {}
    void enqueue(int value, int id);
    int dequeue(int id);

private:
    std::unique_ptr<HashSegment> head_;
    HashSegment* tail_;
    std::unique_ptr<HashSegment> aux_tail_;
    // aux_tail_ will catch early enqueues
};

} // HashQ
} // seven_jalapenos

#endif //HASH_Q_HPP
