
// #include <mutex>
#include <tuple>

#include "balancer.hpp"

namespace seven_jalapenos::CountingNetwork{

Balancer::Balancer(Balancer* up, Balancer* down):
    up_(up), down_(down), count_(0) {}

std::tuple<Balancer*, size_t> Balancer::next() {
    size_t last = count_.fetch_add(1, std::memory_order_relaxed);
    Balancer *next = last % 2 == 0 ? up_ : down_;
    return {next, last};
}

bool Balancer::is_internal() const { return true; }

bool ExternalBalancer::is_internal() const { return false; }

std::tuple<int, size_t> ExternalBalancer::next() {
    size_t last = count_.fetch_add(1, std::memory_order_relaxed);
    int next = last % 2 == 0 ? up_ : down_;
    return {next, last};
}


} // seven_jalapenos::CountingNetwork