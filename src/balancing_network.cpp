//
// Created by jjurgenson on 25/06/20.
//

#include <cassert>
#include <cmath>
#include <format>
#include <memory>
#include <vector>

#include "balancing_network.hpp"

namespace seven_jalapenos::CountingNetwork{

BalancingNetwork::~BalancingNetwork() = default;

BalancingNetwork::BalancingNetwork(int width){
    // width must be a power of 2
    assert((width > 0) && (width & (width - 1)) == 0);
    width_ = width;
    using namespace seven_jalapenos::CountingNetwork;

    int q = std::log2(width_);
    int balancers = (width_/2) * q * (q + 1) / 2;// number of balancers
    balancer_nodes_.resize(balancers);
    entry_wires_.resize(width_);
    int internal = balancers - (width_/2);
    int extern_idx = 0;
    // init all nodes
    for (int i = 0; i < balancers; i++){
        if (i < internal)
            balancer_nodes_[i] = std::make_unique<Balancer>();
        else {
            balancer_nodes_[i] = std::make_unique<ExternalBalancer>(extern_idx);
            extern_idx += 2;
        }
    }

    using std::ref;
    // struct to keep track of wires
    typedef struct NetworkWire{
        Balancer* node_;
        Balancer** wire_;
        bool is_up_;
        int id; // debug

        void set_wire(Balancer* n, bool is_top, int i){
            // connect old node to new node
            is_up_? node_->up_ = n: node_->down_ = n;
            // update wire
            if (is_top){
                wire_ = &n->up_;
                is_up_ = true;
            } else {
                wire_ = &n->down_;
                is_up_ = false;
            }
            // update node
            node_ = n;
            id = i; // debug
        }
        void set_wire(ExternalBalancer* n) {
            is_up_? node_->up_ = n: node_->down_ = n;
        }
    } NW;

    // initial layer of balancers
    int balancer_i = 0;
    std::vector<NetworkWire> tmp_wires(width_);
    for (size_t i = 0; i < entry_wires_.size(); i += 2){
        int next = i + 1;
        int j = balancer_i++;
        Balancer* curr = balancer_nodes_[j].get();
        tmp_wires[i] = NW(curr, &curr->up_, true, j);
        tmp_wires[next] = NW(curr, &curr->down_, false, j);
        entry_wires_[i] = curr;
        entry_wires_[next] = curr;
    }

    int peicewise = 2;
    while (peicewise < width_){
        peicewise <<= 1;
        // funnel section
        for (int j = 0; j < width_; j += peicewise){
            int top = j;
            int low = top + peicewise - 1;
            while (top < low){
                int j = balancer_i++;
                auto curr = balancer_nodes_[j].get();
                tmp_wires[top].set_wire(curr, true, j);
                tmp_wires[low].set_wire(curr, false, j);
                top += 1; low -= 1;
            }
        }
        // merger section
        int block_size = peicewise / 2;
        while(block_size > 2){
            int block_num = width_ / block_size;
            int balancer_length = block_size / 2; //check this
            int balancer_num = balancer_length;
            for (int i = 0; i < block_num; i++){
                for (int j = 0; j < balancer_num; j++){
                    int k = balancer_i++;
                    auto curr = balancer_nodes_[k].get();
                    int top = i * block_size + j;
                    int low = top + balancer_length;
                    tmp_wires[top].set_wire(curr, true, k);
                    tmp_wires[low].set_wire(curr, false, k);
                }
            }
            block_size >>= 1;
        }
        //final balancer layer
        int layer_num = width_ / 2;
        for (int i = 0; i < layer_num; i++) {
            int top = i * 2;
            int low = top + 1;
            int j = balancer_i++;
            auto curr = balancer_nodes_[j].get();
            tmp_wires[top].set_wire(curr, true, j);
            tmp_wires[low].set_wire(curr, false, j);
        }
    }
}

int BalancingNetwork::traverse(int id){
    if (!valid_index(id))
        id = id % width_;
    Balancer* current = entry_wires_[id];
    while(current->is_internal()){
        current = current->next();
    }
    auto pod = dynamic_cast<ExternalBalancer*>(current);
    return pod->next();
}

[[nodiscard]] int BalancingNetwork::width() const { return width_; }

[[nodiscard]] bool BalancingNetwork::valid_index(int id) const { return id < width_ && id % width_ == 0; }


} // seven_jalapenos::CountingNetwork