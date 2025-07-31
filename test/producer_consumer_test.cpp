
#include <algorithm>
// #include <cstddef>
#include <cstdint>
#include <format>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

#include "hash_q.hpp"

int main(){
    using namespace seven_jalapenos::HashQ;
    int length = 32;
    int width = 8;
    int t_num = 2;
    int rep = 17;
    int wait = 0;

    HashQ q(length, width);
    std::vector<std::thread> nq_threads;
    std::vector<std::thread> dq_threads;
    std::mutex io_mutex;
    std::vector<int> rets;

    auto produce = [](HashQ &q, int id, int rep)-> void{
        for (int i = 0; i < rep; i++) {
            q.enqueue((id * rep) + i, id);
        }
    };
    auto consume = [](HashQ &q, int id, int rep, std::vector<int> &rets, std::vector<int> &local_rets, std::mutex &mtx)-> void{
        for (int i = 0; i < rep; i++) {
            local_rets.push_back(q.dequeue(id));
        }
        std::lock_guard lck(mtx);
        for (int i = 0; i < local_rets.size(); i++) {
            rets.push_back(local_rets[i]);
        }
    };

    for (int i = 0; i < t_num; i++) {
        nq_threads.emplace_back([&, i, rep](){
            std::cout << "s" << "\n";
            produce(q, i, rep);
            std::cout << "e" << "\n";
        });
    }
    for (auto & t : nq_threads) {
        t.join();
        // std::cout << std::format("thread {} has joined", std::hash<std::thread::id>{}(std::this_thread::get_id()));
    }

    for(int i = 0; i < t_num; i++){
        dq_threads.emplace_back([&, i, rep](){
            std::vector<int> local_rets;
            consume(q, i, rep, rets, local_rets, io_mutex);
        });
    }
    for (auto & t: dq_threads){
        t.join();
    }
    std::sort(rets.begin(), rets.end());
    for (auto ret: rets) {
        std::cout << ret << "\n";
    }

}