
#include "counting_network.hpp"

#include <atomic>
#include <chrono>
#include <fstream>
#include <iostream>
#include <thread>
#include <vector>


// -----------------------------------------------------------------------------
// Workload configuration
// -----------------------------------------------------------------------------
constexpr size_t TOTAL_OPS   = 1'000'000;   // total increments across all threads
constexpr int    MAX_THREADS = 32;          // you can adjust as needed
constexpr int    MAX_WIDTH   = 16;
constexpr int    RUNS        = 10;

// -----------------------------------------------------------------------------
// Baseline: atomic counter
// -----------------------------------------------------------------------------
double run_atomic_counter(int num_threads) {
    size_t ops_per_thread = TOTAL_OPS / num_threads;
    size_t extra_iter = TOTAL_OPS % num_threads;

    std::chrono::duration<double> total_time{0};
    for (int i = 0; i < RUNS; i++){
        std::atomic<size_t> counter{0};
        std::vector<std::thread> threads;
        threads.reserve(num_threads);

        auto start = std::chrono::high_resolution_clock::now();

        for (int t = 0; t < num_threads; ++t) {
            threads.emplace_back([&, t]() {
                size_t ops = t < extra_iter ? ops_per_thread + 1 : ops_per_thread;
                for (size_t i = 0; i < ops; ++i) {
                    counter.fetch_add(1, std::memory_order_relaxed);
                }
            });
        }
        for (auto& th : threads) th.join();

        auto end   = std::chrono::high_resolution_clock::now();
        total_time += end - start;
    }

    double avg = total_time.count() / RUNS;
    double mops = static_cast<double>(TOTAL_OPS) / (avg * 1e6);
    return mops;
}

// -----------------------------------------------------------------------------
// Test target: counting_net (user provides implementation)
// -----------------------------------------------------------------------------

double run_counting_net(int num_threads, int width) {
    using counting_net = seven_jalapenos::CountingNetwork::CountingNetwork;
    size_t ops_per_thread = TOTAL_OPS / num_threads;
    size_t extra_iter = TOTAL_OPS % num_threads;

    std::chrono::duration<double> total_time{0};
    for (int i = 0; i < RUNS; i++){
        counting_net counter(width);
        std::vector<std::thread> threads;
        threads.reserve(num_threads);

        auto start = std::chrono::high_resolution_clock::now();

        for (int t = 0; t < num_threads; ++t) {
            threads.emplace_back([&, t]() {
                size_t ops = t < extra_iter ? ops_per_thread + 1 : ops_per_thread;
                for (size_t i = 0; i < ops; ++i) {
                    counter.get_and_increment(t);
                }
            });
        }
        for (auto& th : threads) th.join();

        auto end   = std::chrono::high_resolution_clock::now();
        total_time += end - start;
    }

    double avg = total_time.count() / RUNS;
    double mops = static_cast<double>(TOTAL_OPS) / (avg * 1e6);
    return mops;
}
// (final=" << counter.value() << ")
// -----------------------------------------------------------------------------
// Driver
// -----------------------------------------------------------------------------
int main() {
    std::ofstream outfile("/home/jjurgenson/code_stuff/CombiningCounterTest/test/counter_results.csv"); 
    outfile << "threads atomic 4 8 16\n";
    for (int num_threads = 1; num_threads <= MAX_THREADS; num_threads ++) {
        outfile << num_threads;
        double counter_mops = run_atomic_counter(num_threads);
        outfile << " " << counter_mops;
        std::cout << "ran atomic threads = " << num_threads << "\n";
        for(int width = 4; width <= MAX_WIDTH; width *= 2){
            double net_mops = run_counting_net(num_threads, width);
            outfile << " " << net_mops;
            std::cout << "ran net width = " << width << " tnum = " << num_threads << "\n";
        }
        outfile << std::endl;
    }
    return 0;
}
