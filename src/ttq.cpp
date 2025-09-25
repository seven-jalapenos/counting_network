
#include "ttq.hpp"
#include <cstddef>
#include <stdexcept>

namespace seven_jalapenos::TTQ{

TTQ::TTQ(size_t width, size_t length): width(width), length(length), 
         ring_(length), nq_net_(width), dq_net_(width) {}

bool TTQ::enqueue(int value, int id){
    auto [idx, epoch] = nq_net_.get_and_increment(id);
    size_t ring_idx = ring_.get_idx(idx);
    auto cell = &ring_.arr[ring_idx];
    if (cell->empty == false){
        return false;
    }
    cell->elt = value;
    cell->empty = false;
    return true;
}

int TTQ::dequeue(int id){
    auto [idx, epoch] = dq_net_.get_and_increment(id);
    size_t ring_idx = ring_.get_idx(idx);
    auto cell = &ring_.arr[ring_idx];
    if (cell->empty == true){
        throw std::runtime_error("dequeue on empty queue");
    }
    int value = cell->elt;
    cell->empty = true;
    return value;
}





} // seven_jalapenos::TTQ