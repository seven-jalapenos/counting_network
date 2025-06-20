

#ifndef BALANCER_NODE
#define BALANCER_NODE

#include <mutex>
#include <memory>

namespace seven_jalapenos::CountingNetwork{

class Node{
public:
    std::mutex mtx_;
    virtual Node* next() = 0;
    [[nodiscard]] virtual bool is_internal() const = 0;
    virtual ~Node();
};

class Balancer final : public Node{
public:
    Node* up_;
    Node* down_;
    bool go_up_;

    explicit Balancer(Node* up=nullptr, Node* down=nullptr);
    Node* next() override;
    [[nodiscard]] bool is_internal() const override;
};

template<typename T>
class ExternalBalancer final : public Node{
public:
    // explicit OutputNode(T elt): point_(std::make_unique<T>(elt)) {}
    explicit ExternalBalancer(int idx): point_(new T), idx_(idx) {}

    T* get_p() { return point_.get(); }
    [[nodiscard]] int get_idx() const { return idx_; }
    Node* next() override { throw std::logic_error("not implemented"); }
    [[nodiscard]] bool is_internal() const override { return false; }

private:
    std::unique_ptr<T> point_;
    int idx_;
};

} // seven_jalapenos::counting_network

#endif // BALANCER_NODE