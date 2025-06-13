
#pragma once

#ifndef BALANCER_NODE
#define BALANCER_NODE

#include <mutex>
#include <memory>

class Node{
public:
    virtual Node* next() = 0;
    virtual bool is_internal() const = 0;
    virtual ~Node();
};

class Balancer final : public Node{
public:
    Node* up_;
    Node* down_;
    bool go_up_;
    std::mutex mtx_;

    Balancer(Node* up=nullptr, Node* down=nullptr);
    Node* next() override;
    [[nodiscard]] bool is_internal() const override;
};

template<typename T>
class OutputNode final : public Node{
public:
    // explicit OutputNode(T elt): point_(std::make_unique<T>(elt)) {}
    OutputNode(): point_(new T) {}

    T* get_p() { return point_.get(); }
    Node* next() override { throw std::logic_error("not implemented"); }
    [[nodiscard]] bool is_internal() const override { return false; }

private:
    std::unique_ptr<T> point_;
};

#endif