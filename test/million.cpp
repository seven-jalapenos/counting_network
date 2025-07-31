
#include <algorithm>
#include <chrono>
#include <iostream>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>
#include <cassert>

#include "hash_q.hpp"

// int seive(int work){
//     int n = work;
//     std::vector<bool> prime(n, true);
//     for (int p = 2; p * p < n; ++p) {
//         if (prime[p]) {
//             for (int i = p * p; i < n; i += p)
//                 prime[i] = false;
//         }
//     }
//     int num = 0;
//     std::vector<int> res(n, -1);
//     for (size_t i = 0; i < prime.size(); i++)
//     {
//         if (prime[i]){
//             res.push_back(i);
//             num++;
//         }
//     }
//     res.resize(num);
//     return res.size();
// }

int main(int argc, char* argv[]){
    typedef seven_jalapenos::HashQ::HashQ HashQ;
    // std::mutex io_mtx;
    // size_t global_thread_id = 0;
    int million = 1000000;
    int runs = 10;
    int warm_ups = 3;
    int num_processor = 12;
    int num_threads = 12;
    int length = 64;
    int width = 16;
    switch (argc) {
    case 5:
        width = std::stoi(argv[4]);
        [[fallthrough]];
    case 4:
        length = std::stoi(argv[3]);
        [[fallthrough]];
    case 3:
        num_threads = std::stoi(argv[2]);
        [[fallthrough]];
    case 2:
        num_processor = std::stoi(argv[1]);
        [[fallthrough]];
    case 1:
        break;
    default:
        throw(std::runtime_error("invalid number of inline arguments"));
    }

    std::vector<std::thread> threads;
    int iter_per_thread = million / num_threads;
    int extra_iter = million % num_threads;

    // auto set_cpu = [=](std::thread& t, int id){
    //     int core_i = id / thread_per_processor;
    //     cpu_set_t set;
    //     CPU_ZERO(&set);
    //     CPU_SET(core_i, &set);
    //     int rc = pthread_setaffinity_np(t.native_handle(), sizeof(set), &set);
    //     if(rc != 0){
    //         std::cerr << "error calling pthread_setaffinity_np: " << rc << std::endl;
    //     }
    //     // return 0;
    // };

    auto enqueue_group = [num_threads](int iters, HashQ& q, int tid)->void{
        for (int i = 0; i < iters; i++) {
            q.enqueue(tid * num_threads + i, tid);
        }
        // std::lock_guard<std::mutex> lock(io_mtx);
        // std::cout << "combinc thread #" << global_thread_id++ << " running on cpu " << sched_getcpu() << "\n";
    };

    // warm up
    for (int i = 0; i < warm_ups; i++){
        HashQ q(length, width);
        for (int ii = 0; ii < num_threads; ii++){
            int iters = ii < extra_iter ? iter_per_thread + 1 : iter_per_thread;
            threads.emplace_back(enqueue_group, iters, std::ref(q), ii);
        }
        for (auto & t: threads){
            t.join();
        }
    }

    using namespace std::chrono;
    duration<double> total_time; 
    for (int i = 0; i < runs; i++){
        HashQ q(length, width);
        auto start = high_resolution_clock::now();
        for (int ii = 0; ii < num_threads; ii++){
            int iters = ii < extra_iter ? iter_per_thread + 1 : iter_per_thread;
            threads.emplace_back(enqueue_group, iters, std::ref(q), ii);
        }
        for (auto & t: threads){
            t.join();
        }
        auto end = high_resolution_clock::now();
        total_time += end - start;
    }
    double avg_time = total_time.count() / runs;
    double megaops_per_sec = million / avg_time;


    std::cout << "megaops per second: " << megaops_per_sec << std::endl;
    // std::cout << "\n" << "global thread id: " << global_thread_id << " theoretically correct threads " << ((warm_ups * num_thread) + (runs * num_thread)) * 2 << "\n";

    return 0;
}