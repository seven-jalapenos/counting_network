
#include <algorithm>
// #include <cstddef>
#include <format>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

#include "hash_q.hpp"

int main(){
    using namespace seven_jalapenos::HashQ;
    int length = 12;
    int width = 4;
    int t_num = 2;
    int rep = 7;
    int wait = 0;

    HashQ q(length, width);
    std::vector<std::thread> threads;
    std::mutex io_mutex;
    std::vector<int> rets;

    auto op_pair = [](HashQ &q, int id, int rep, std::vector<int> &rets,
                                std::vector<int> &local_rets, std::mutex &mtx)-> void{
        for (int i = 0; i < rep; i++) {
            q.enqueue((id * rep) + i, id);
            local_rets.push_back(q.dequeue(id));
        }
        std::lock_guard lck(mtx);
        for (int i = 0; i < local_rets.size(); i++) {
            rets.push_back(local_rets[i]);
        }
    };

    for(int i = 0; i < t_num; i++){
        threads.emplace_back([&, i, rep](){
            std::vector<int> local_rets;
            op_pair(q, i, rep, rets, local_rets, io_mutex);
        });
    }
    for (auto & t: threads){
        t.join();
    }
    // std::sort(rets.begin(), rets.end());
    for (auto ret: rets) {
        std::cout << ret << "\n";
    }

}