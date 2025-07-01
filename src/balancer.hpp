

#ifndef BALANCER_NODE
#define BALANCER_NODE

#include <tuple>

#include <atomic>
// #include <memory>

namespace seven_jalapenos::CountingNetwork{

// class Node{
// public:
//     std::mutex mtx_;
//     virtual Node* next() = 0;
//     [[nodiscard]] virtual bool is_internal() const = 0;
//     virtual ~Node();
// };

class Balancer{
public:
    virtual ~Balancer() = default;

    Balancer* up_;
    Balancer* down_;
    std::atomic<size_t> count_;

    explicit Balancer(Balancer* up=nullptr, Balancer* down=nullptr);
    std::tuple<Balancer*, size_t> next();
    [[nodiscard]] virtual bool is_internal() const;
};

class ExternalBalancer final : public Balancer{
public:
    int up_;
    int down_;
    std::atomic<size_t> count_;
    // explicit OutputNode(T elt): point_(std::make_unique<T>(elt)) {}
    explicit ExternalBalancer(int idx): up_(idx), down_(idx + 1), count_(0) {}
    int next();
    [[nodiscard]] bool is_internal() const override;
};

} // seven_jalapenos::counting_network

#endif // BALANCER_NODE