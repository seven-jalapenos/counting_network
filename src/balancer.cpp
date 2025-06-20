
#include <mutex>

#include "balancer.hpp"

namespace seven_jalapenos::CountingNetwork{

Balancer::Balancer(Balancer* up, Balancer* down):
    up_(up), down_(down), go_up_(true) {}

Balancer* Balancer::next() {
    std::lock_guard lock(mtx_);
    Balancer *next = go_up_ ? up_ : down_;
    go_up_ = !go_up_;
    return next;
}

bool Balancer::is_internal() const { return true; }

bool ExternalBalancer::is_internal() const { return false; }

int ExternalBalancer::next() {
    std::lock_guard lock(mtx_);
    int next = go_up_ ? up_ : down_;
    go_up_ = !go_up_;
    return next;
}


} // seven_jalapenos::CountingNetwork