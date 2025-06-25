//
// Created by jjurgenson on 25/06/19.
//

#include <iostream>
#include <thread>
#include <format>

#include "counting_network.hpp"

#include <cassert>


int main() {
    typedef seven_jalapenos::CountingNetwork::CountingNetwork Network;
    int width = 4;
    int t_num = 1000;
    int reps = 1;
    int wait = 5;
    Network net(width);
    std::thread threads[t_num];
    std::mutex io_mutex;
    std::vector<std::string> rets;
    std::vector<std::string> errs;

    auto get_inc = [&, width] (int id, int rep, Network& net) -> void {
        size_t counts[rep];
        for (int i = 0; i < rep; i++){
            rets[i] = net.get_and_increment(id);
        }
        std::lock_guard lock(io_mutex);
        for (int i = 0; i < rep; i++){
            size_t res = counts[i];
            std::string str = std::format("t{}: {}", id, res);
            if (id != res / i)
                 errs.push_back(str);
            rets.push_back(str);
        }
    };

    for(int i = 0; i < t_num; ++i) {
        threads[i] = std::thread(get_inc, i, reps, std::ref(net));
        std::this_thread::sleep_for(std::chrono::nanoseconds(wait));
    }
    for(auto & t : threads) {
        t.join();
    }
    for (auto ret: rets) {
        std::cout << ret << "\n";
    }
    std::cout << "\n";
    for (auto err: errs) {
        std::cout << err << "\n";
    }

}
