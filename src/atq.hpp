
#ifndef ATQ_HPP
#define ATQ_HPP

#include "ring_array.hpp"
#include <atomic>
#include <cstddef>

namespace seven_jalapenos::TTQ{ // tile ticket queue

class ATQ{

public:
    const size_t length;

    ATQ(size_t length);
    bool enqueue(int value);
    int dequeue();

private:
    std::atomic<size_t> nq_counter;
    std::atomic<size_t> dq_counter;
    RingArray ring_;

};



} // seven_jalapenos::TTQ





#endif // ATQ_HPP