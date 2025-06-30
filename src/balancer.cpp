
// #include <mutex>

#include "balancer.hpp"

namespace seven_jalapenos::CountingNetwork{

Balancer::Balancer(Balancer* up, Balancer* down):
    up_(up), down_(down), count_(0) {}

Balancer* Balancer::next() {
    size_t old = count_.fetch_add(1, std::memory_order_relaxed);
    Balancer *next = old % 2 == 0 ? up_ : down_;
    return next;
}

bool Balancer::is_internal() const { return true; }

bool ExternalBalancer::is_internal() const { return false; }

int ExternalBalancer::next() {
    size_t old = count_.fetch_add(1, std::memory_order_relaxed);
    int next = old % 2 == 0 ? up_ : down_;
    return next;
}


} // seven_jalapenos::CountingNetwork