//
// Created by jjurgenson on 25/06/19.
//

#include <algorithm>
#include <cstddef>
#include <iostream>
#include <thread>
#include <format>

#include "counting_network.hpp"

#include <cassert>

struct Dequeue {
    size_t number;
    int t_id;
    size_t accum;
};

std::ostream& operator<<(std::ostream &stream, const Dequeue &dq){
    return stream << std::format("t{}: {}, {}", dq.t_id, dq.number, dq.accum);
}

int main() {
    typedef seven_jalapenos::CountingNetwork::CountingNetwork Network;
    int width = 4;
    int t_num = 5;
    int reps = 16;
    int wait = 0;
    Network net(width);
    std::thread threads[t_num];
    std::mutex io_mutex;
    std::vector<Dequeue> rets;
    std::vector<Dequeue> errs;

    auto get_inc = [&, width] (const int id, const int rep, Network& net) -> void {
        std::tuple<size_t, size_t> counts[rep];
        for (int i = 0; i < rep; i++){
            counts[i]  = net.get_and_increment(id);
        }
        std::lock_guard lock(io_mutex);
        for (int i = 0; i < rep; i++){
            auto [count, acc] = counts[i];
            Dequeue dq {count, id, acc};
            if (id != count / rep){
                 errs.push_back(dq);
            }
            rets.push_back(dq);
        }
    };

    for(int i = 0; i < t_num; ++i) {
        threads[i] = std::thread(get_inc, i, reps, std::ref(net));
        std::this_thread::sleep_for(std::chrono::nanoseconds(wait));
    }
    for(auto & t : threads) {
        t.join();
    }

    auto sort_dq = [](Dequeue dq1, Dequeue dq2)-> bool { return dq1.number < dq2.number; };
    std::sort(rets.begin(), rets.end(), sort_dq);
    std::sort(errs.begin(), errs.end(), sort_dq);
    for (auto ret: rets) {
        std::cout << ret << "\n";
    }
    std::cout << "\n";
    for (auto err: errs) {
        std::cout << err << "\n";
    }

}
