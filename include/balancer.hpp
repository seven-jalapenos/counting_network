
#pragma once

#include <mutex>

class Node{
public:
    std::mutex mtx_;
    Node* next();
    virtual bool is_internal() const = 0;
    // virtual ~Node() = default;
};

class Balancer: public Node{
public:
    Node* up_;
    Node* down_;

    Balancer(Node* up=nullptr, Node* down=nullptr):
        up_(up), down_(down), go_up_(true) {}
    Node* next();
    bool is_internal() const override {return true;}

private:
    bool go_up_;
};

template<typename T>
class OutputNode: public Node{
public:
    T elt_;

    OutputNode<T>(T elt): elt_(elt) {}
    OutputNode<T>(): elt_(nullptr) {}
    // T access();
    Node* next() = delete;
    bool is_internal() const override {return false;}
};