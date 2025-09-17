//
// Created by jjurgenson on 25/06/20.
//

#include "hash_q.hpp"
#include "hash.hpp"
#include <atomic>
#include <memory>

namespace seven_jalapenos {
namespace HashQ {

HashQ::HashQ(int length, int width):
    net_width(width), hash_length(length), size(1),
    nq_net(width), dq_net(width),
    head_(new HashSegment(length)), tail_(head_.get()) 
{
    tail_.load()->next = std::make_unique<HashSegment>(hash_length);
    aux_tail_ = tail_.load()->next.get();
}

int HashQ::dequeue(int id) {
    // change phase to dequeue
    // if (head_->enqueue_phase.load())
    //     head_->enqueue_phase.store(false);

    auto [key, acc] = dq_net.get_and_increment(id);
    HashSegment* next = size.load(std::memory_order_acquire) > 1 ?
                        head_->next.get() : nullptr;
    // if we are slightly early we can dequeue first >>
    if (!head_->hash.valid_dq(key)){
        if (size.load(std::memory_order_acquire) == 1)
            throw std::runtime_error("empty Queue exception");
        int ret = next->hash.get(key);
        return ret;
    }
    int ret = head_->hash.get(key);
    // thread to dequeue last elt moves head_ >>thread free
    if (key % hash_length == hash_length - 1 &&
        size.load(std::memory_order_acquire) > 1) {
        while(!head_->hash.is_empty());
        head_ = std::move(head_->next);
        size.fetch_add(-1, std::memory_order_release);
    }
    return ret;
}

void HashQ::enqueue(int value, int id) {
    // if (!head_->enqueue_phase.load()){
        
    // }

    auto [key, acc] = nq_net.get_and_increment(id);
    HashSegment* aux = aux_tail_.load();
    // threads will enqueue at aux_tail_ if early or tail_ is being reset >>
    if (!tail_.load()->hash.put(key, value)){
        aux->hash.put(key, value);
    }
    // thread to enqueue last elt will move tail_ and reset aux_tail_ >>
    if (key % hash_length == hash_length - 1){
        while(!tail_.load()->hash.is_full());
        // tail_->next = std::move(aux_tail_);
        HashSegment* tmp = aux_tail_.load();
        tmp->next = std::make_unique<HashSegment>(hash_length); 
        aux_tail_.store(tmp->next.get());
        tail_ = tmp;
        size.fetch_add(1, std::memory_order_release);
    }
}

} // HashQ
} // seven_jalapenos