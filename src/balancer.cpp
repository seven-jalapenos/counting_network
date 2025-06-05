
#include <mutex>

#include "balancer.hpp"

Node* Balancer::next(){
    std::lock_guard<std::mutex> lock(mtx_);
    Node* next = go_up_ == true ? up_ : down_;
    go_up_ = !go_up_;
    return next;
}