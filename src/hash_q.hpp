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
    void enqueue(int value);
    int dequeue();

private:
    HashSegment* head_;
    HashSegment* tail_;


};

} // HashQ
} // seven_jalapenos

#endif //HASH_Q_HPP
