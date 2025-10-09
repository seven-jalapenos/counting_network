
#include <algorithm>
#include <chrono>
#include <iostream>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>
#include <cassert>

#include "ttq.hpp"

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
    typedef seven_jalapenos::TTQ::TTQ TTQ;
    // std::mutex io_mtx;
    // size_t global_thread_id = 0;
    int million = 1000000;
    int runs = 10;
    int warm_ups = 3;
    int num_processor = 12;
    int num_threads = 32;
    int length = 64;
    int width = 8;
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

    auto enqueue_group = [num_threads](int iters, TTQ* q, int tid)->void{
        for (int i = 0; i < iters; i++) {
            q->enqueue(tid + i * num_threads, tid);
        }
        // std::lock_guard<std::mutex> lock(io_mtx);
        // std::cout << "combinc thread #" << global_thread_id++ << " running on cpu " << sched_getcpu() << "\n";
    };

    auto dequeue_group = [num_threads](int iters, TTQ* q, int tid)->void{
        for (int i = 0; i < iters; i++) {
            q->dequeue(tid);
        }
    };

    // auto mixed_group = [num_threads](int iters, TTQ* q, int tid)->void{
    //     for (int i = 0; i < iters)
    // };

    // warm up
    for (int i = 0; i < warm_ups; i++){
        auto q = std::make_unique<TTQ>(width, million);
        std::vector<std::thread> threads;
        for (int ii = 0; ii < num_threads; ii++){
            int iters = ii < extra_iter ? iter_per_thread + 1 : iter_per_thread;
            threads.emplace_back(enqueue_group, iters, q.get(), ii);
        }
        for (auto & t: threads){
            t.join();
        }
        threads.clear();
        for (int ii = 0; ii < num_threads; ii++){
            int iters = ii < extra_iter ? iter_per_thread + 1 : iter_per_thread;
            threads.emplace_back(dequeue_group, iters, q.get(), ii);
        }
        for (auto & t: threads){
            t.join();
        }
    }

    using namespace std::chrono;
    duration<double> total_nq_time; 
    duration<double> total_dq_time;
    for (int i = 0; i < runs; i++){
        auto q = std::make_unique<TTQ>(width, million);

        // enqueue group
        std::vector<std::thread> nq_threads;
        auto start = high_resolution_clock::now();
        for (int ii = 0; ii < num_threads; ii++){
            int iters = ii < extra_iter ? iter_per_thread + 1 : iter_per_thread;
            nq_threads.emplace_back(enqueue_group, iters, q.get(), ii);
        }
        for (auto & t: nq_threads){
            t.join();
        }
        auto end = high_resolution_clock::now();
        total_nq_time += end - start;

        // dequeue group
        std::vector<std::thread>dq_threads;
        start = high_resolution_clock::now();
        for (int ii = 0; ii < num_threads; ii++){
            int iters = ii < extra_iter ? iter_per_thread + 1 : iter_per_thread;
            dq_threads.emplace_back(dequeue_group, iters, q.get(), ii);
        }
        for (auto & t: dq_threads){
            t.join();
        }
        end = high_resolution_clock::now();
        total_dq_time += end - start;

    }

    //enqueue time
    double avg_nq_time = total_nq_time.count() / runs;
    double megaops_per_sec_nq = 1 / avg_nq_time;

    //dequeue time
    double avg_dq_time = total_dq_time.count() / runs;
    double megaops_per_sec_dq = 1 / avg_dq_time;


    std::cout << "megaops per second (enqueue): " << megaops_per_sec_nq << "\n";
    std::cout << "megaops per second (dequeue): " << megaops_per_sec_dq << "\n";
    // std::cout << "\n" << "global thread id: " << global_thread_id << " theoretically correct threads " << ((warm_ups * num_thread) + (runs * num_thread)) * 2 << "\n";

    return 0;
}