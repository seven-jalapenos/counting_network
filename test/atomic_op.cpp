
#include <iostream>
#include <thread>
#include <vector>

#include "atq.hpp"

constexpr int MILLION = 1'000'000;

double atomic_bm(int num_threads, int warm_up=3, int runs=10){
    using namespace seven_jalapenos::TTQ;
    // std::mutex io_mutex;
    std::vector<int> per_thread_reps(num_threads);
    int iter_per_thread = (MILLION / 2) / num_threads;
    int extra_iter = (MILLION / 2) % num_threads;

    auto even_mixed_op = [num_threads](ATQ *q, int id, int rep){
        // enqueue
        for (int i = 0; i < rep; i++) {
            q->enqueue(id + i * num_threads);
        }
        // dequeue
        for (int i = 0; i < rep; i++) {
            q->dequeue();
        }
    };

    for (int i = 0; i < warm_up; i++) {
        ATQ q(MILLION);
        std::vector<std::thread> threads;

        for (int ii = 0; ii < num_threads; ii++) {
            int iters  = ii < extra_iter ? iter_per_thread + 1 : iter_per_thread;
            per_thread_reps[ii] = iters;
            threads.emplace_back(even_mixed_op, &q, ii, iters);
        }
        for (auto & t : threads) {
            t.join();
            // std::cout << std::format("thread {} has joined", std::hash<std::thread::id>{}(std::this_thread::get_id()));
        }
    }

    using namespace std::chrono;
    duration<double> total_time;
    for (int i = 0; i < runs; i++) {
        ATQ q(MILLION);
        std::vector<std::thread> threads;

        auto start = high_resolution_clock::now();
        for (int ii = 0; ii < num_threads; ii++) {
            threads.emplace_back(even_mixed_op, &q, num_threads, per_thread_reps[ii]);
        }
        for (auto & t : threads) {
            t.join();
        }
        auto end = high_resolution_clock::now();
        total_time += end - start;

    }
    
    double avg_time = total_time.count() / runs;
    double megaops_per_sec_ttq = 1 / avg_time;
    return megaops_per_sec_ttq;
}

int main(int argc, char* argv[]){
    // int length = 32;
    int num_threads = 32;
    int runs = 10;
    int warm_up = 3;
    int wait = 0;
    switch (argc) {
    case 2:
        num_threads = std::stoi(argv[1]);
        [[fallthrough]];
    case 1:
        break;
    default:
        throw(std::runtime_error("invalid number of inline arguments"));
    }

    double megaops_per_sec = 0;

    megaops_per_sec = atomic_bm(num_threads);

    std::cout << megaops_per_sec << std::endl;
}
