
#include <mutex>

#include "node.hpp"

Node::~Node() = default;

Balancer::Balancer(Node* up, Node* down):
    up_(up), down_(down), go_up_(true) {}

Node* Balancer::next(){
    std::lock_guard lock(mtx_);
    Node* next = go_up_ ? up_ : down_;
    go_up_ = !go_up_;
    return next;
}

bool Balancer::is_internal() const { return true; }
