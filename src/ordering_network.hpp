
#ifndef ORDERING_NETWORK_H
#define ORDERING_NETWORK_H

#include <memory>
#include <vector>

#include "balancer.hpp"

namespace seven_jalapenos::CountingNetwork{

class OrderingNetwork{
public:
    explicit OrderingNetwork(int width);

    int traverse(int id);

    [[nodiscard]] int width() const;

protected:
    int width_;
    std::vector<std::unique_ptr<Balancer>> balancer_nodes_;
    std::vector<Balancer*> entry_wires_;

    [[nodiscard]] bool valid_index(int i) const;
};

} // seven_jalapenos::counting_network

#endif // ORDERING_NETWORK_H