//
// Created by jjurgenson on 25/06/20.
//

#include "hash_q.hpp"
#include "hash.hpp"

namespace seven_jalapenos {
namespace HashQ {

int HashQ::dequeue(int id) {
    auto [key, acc] = dq_net.get_and_increment(id);
    HashSegment* next = size > 1 ? head_->next.get() : nullptr;
    // if we are slightly early we can dequeue first >>
    if (!head_->hash.valid_dq(key)){
        if (size == 1)
            throw std::runtime_error("empty Queue exception");
        int ret = next->hash.get(key);
        return ret;
    }
    int ret = head_->hash.get(key);
    // thread to dequeue last elt moves head_ >>
    if (key % hash_length == hash_length - 1 && size > 1) {
        while(!head_->hash.is_empty());
        head_ = std::move(head_->next);
        size--;
    }
    return ret;
}

void HashQ::enqueue(int value, int id) {
    auto [key, acc] = nq_net.get_and_increment(id);
    HashSegment* aux = aux_tail_.get();
    // threads will enqueue at aux_tail_ if early or tail_ is being reset >>
    if (!tail_->hash.put(key, value)){
        aux->hash.put(key, value);
    }
    // thread to enqueue last elt will move tail_ and reset aux_tail_ >>
    if (key % hash_length == hash_length - 1){
        while(!tail_->hash.is_full());
        tail_->next = std::move(aux_tail_);
        aux_tail_ = std::make_unique<HashSegment>(hash_length);
        size++;
    }
}


} // HashQ
} // seven_jalapenos