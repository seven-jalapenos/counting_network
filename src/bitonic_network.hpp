
#pragma once

#include <cassert>
#include <cmath>
#include <format>
#include <memory>
#include <vector>
#include <iostream>

#include "node.hpp"

template <typename T>
class BitonicNetwork{
public:
    explicit BitonicNetwork(int width){
        // width must be a power of 2
        assert((width > 0) && (width & (width - 1)) == 0);
        width_ = width;

        int q = std::log2(width_);
        int balancers = (width_/2) * q * (q + 1) / 2;// number of balancers
        balancer_nodes_.resize(balancers);
        entry_wires_.resize(width_);
        output_nodes_.resize(width_);
        // init all nodes
        for (auto & balancer_node : balancer_nodes_){
            balancer_node = std::make_unique<Balancer>();
        }
        // init output nodes
        for (size_t i = 0; i < output_nodes_.size(); i++){
            output_nodes_[i] = std::make_unique<OutputNode<T>>();
        }

        using std::ref;
        // struct to keep track of wires
        typedef struct NetworkWire{
            Balancer* node_;
            Node** wire_;
            bool is_up_;

            void set_wire(Balancer* n, bool is_top){
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
            }
            void set_wire(OutputNode<T>* n){
                is_up_? node_->up_ = n: node_->down_ = n;
            }
        } NW;

        // initial layer of balancers
        int balancer_i = 0;
        std::vector<NetworkWire> tmp_wires(width_);
        for (size_t i = 0; i < entry_wires_.size(); i += 2){
            int next = i + 1;
            Balancer* curr = balancer_nodes_[balancer_i++].get();
            tmp_wires[i] = NW(curr, &curr->up_, true);
            tmp_wires[next] = NW(curr, &curr->down_, false);
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
                    auto curr = balancer_nodes_[balancer_i++].get();
                    tmp_wires[top].set_wire(curr, true);
                    tmp_wires[low].set_wire(curr, false);
                    top += 1; low -= 1;
                }
            }
            // merger section
            int m_balancer_length = peicewise / 4;
            while(m_balancer_length > 1){
                int m_block_size = m_balancer_length * 2; //check this
                int m_block_num = width_ / m_balancer_length;
                for (int i = 0; i < m_block_num; i++){
                   for (int j = 0; j < m_balancer_length; j++){
                        auto curr = balancer_nodes_[balancer_i++].get();
                        int top = i * m_block_size + j;
                        int low = top + m_balancer_length;
                        tmp_wires[top].set_wire(curr, true);
                        tmp_wires[low].set_wire(curr, false);
                    }
                }
                m_balancer_length >>= 1;
            }
            //final balancer layer
            int layer_num = width_ / 2;
            for (int i = 0; i < layer_num; i++) {
                int top = i * 2;
                int low = top + 1;
                auto curr = balancer_nodes_[balancer_i++].get();
                tmp_wires[i].set_wire(curr, true);
                tmp_wires[low].set_wire(curr, false);
            }
        }
        //connect to ouptut wires
        for (size_t i = 0; i < tmp_wires.size(); i ++) {
            tmp_wires[i].set_wire(output_nodes_[i].get());
        }
    }

    T* traverse(int id){
        if (!valid_index(id))
            throw std::logic_error(std::format("invalid index of {} for network of width {}", id, width_));
        
        Node* current = entry_wires_[id];
        while(current->is_internal()){
            current = current->next();
        }

        auto pod = dynamic_cast<OutputNode<T>*>(current);
        return pod->get_p();
    }

    T* get_elt(int id) {
        if (!valid_index(id))
            throw std::logic_error(std::format("invalid index of {} for network of width {}", id, width_));

        return output_nodes_[id]->get_p();
    }

    int width() const { return width_; }

private:
    int width_;
    std::vector<std::unique_ptr<Balancer>> balancer_nodes_;
    std::vector<Balancer*> entry_wires_;
    std::vector<std::unique_ptr<OutputNode<T>>> output_nodes_;

    bool valid_index(int i) const {
        return i >= 0 && i < width_;
    }
};