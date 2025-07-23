
#include <algorithm>
// #include <cstddef>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

#include "hash_q.hpp"

int main(){
    using namespace seven_jalapenos::HashQ;
    int length = 64;
    int width = 8;
    int t_num = 4;
    int rep = 16;
    int wait = 0;

    HashQ q(length, width);
    std::vector<std::thread> threads;
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
        for (auto & ret : local_rets) {
            rets.push_back(ret);
        }
    };

    for (int i = 0; i < t_num; i++) {
        threads.emplace_back([&, i, rep](){
            std::vector<int> local_rets;
            produce(q, i, rep);
            consume(q, i, rep, rets, local_rets, io_mutex);
        });
    }
    for (auto & t : threads) {
        t.join();
    }
    std::sort(rets.begin(), rets.end());
    for (auto ret: rets) {
        std::cout << ret << "\n";
    }

}
