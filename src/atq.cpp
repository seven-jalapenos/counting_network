
#include "atq.hpp"
#include <atomic>
#include <cstddef>
#include <stdexcept>

namespace seven_jalapenos::TTQ{

ATQ::ATQ(size_t length): length(length), ring_(length),
                         nq_counter(0), dq_counter(0) {}

bool ATQ::enqueue(int value){
    size_t idx = nq_counter.fetch_add(1, std::memory_order_relaxed);
    size_t ring_idx = ring_.get_idx(idx);
    auto cell = &ring_.arr[ring_idx];
    if (cell->empty.load(std::memory_order_acquire) == false){
        throw std::runtime_error("enqueue on full queue");
    }
    // std::atomic_thread_fence(std::memory_order_acquire);
    cell->elt = value;
    cell->empty.store(false, std::memory_order_release);
    return true;
}

int ATQ::dequeue(){
    size_t idx = dq_counter.fetch_add(1, std::memory_order_relaxed);
    size_t ring_idx = ring_.get_idx(idx);
    auto cell = &ring_.arr[ring_idx];
    if (cell->empty.load(std::memory_order_acquire) == true){
        throw std::runtime_error("dequeue on empty queue");
    }
    // std::atomic_thread_fence(std::memory_order_acquire);
    int value = cell->elt;
    cell->empty.store(true, std::memory_order_release);
    return value;
}





} // seven_jalapenos::ATQ