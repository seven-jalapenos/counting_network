
#ifndef BALANCING_NETWORK
#define BALANCING_NETWORK

#include <memory>
#include <vector>

#include "balancer.hpp"

namespace seven_jalapenos::CountingNetwork{

class BalancingNetwork{
public:
    explicit BalancingNetwork(int width);

    ~BalancingNetwork();

    int traverse(int id);

    [[nodiscard]] virtual int width() const;

protected:
    int width_;
    std::vector<std::unique_ptr<Balancer>> balancer_nodes_;
    std::vector<Balancer*> entry_wires_;

    [[nodiscard]] bool valid_index(int i) const;
};

} // seven_jalapenos::counting_network

#endif // ORDERING_NETWORK_H