
#pragma once

#include <mutex>

class Node{
public:
    Node* next();
    virtual bool is_internal() const = 0;
    // virtual ~Node() = default;
};

class Balancer: public Node{
public:
    Node* up_;
    Node* down_;
    bool go_up_;
    std::mutex mtx_;

    Balancer(Node* up=nullptr, Node* down=nullptr):
        up_(up), down_(down), go_up_(true) {}
    Node* next();
    bool is_internal() const override {return true;}
};

template<typename T>
class OutputNode: public Node{
public:
    T elt_;

    OutputNode(T elt): elt_(elt) {}
    OutputNode(): elt_(nullptr) {}
    Node* next() = delete;
    bool is_internal() const override {return false;}
};