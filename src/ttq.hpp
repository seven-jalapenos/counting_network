
#ifndef TTQ_HPP
#define TTQ_HPP

#include "ring_array.hpp"
#include "counting_network.hpp"
#include <cstddef>

namespace seven_jalapenos::TTQ{ // tile ticket queue

class TTQ{

public:
    const size_t width;
    const size_t length;

    TTQ(size_t width, size_t length);
    bool enqueue(int value, int id);
    int dequeue(int id);

private:
    seven_jalapenos::CountingNetwork::CountingNetwork nq_net_;
    seven_jalapenos::CountingNetwork::CountingNetwork dq_net_;
    RingArray ring_;

};



} // seven_jalapenos::TTQ





#endif // TTQ_HPP