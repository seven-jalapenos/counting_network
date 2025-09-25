
#include "counting_network.hpp"

#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>


// -----------------------------------------------------------------------------
// Workload configuration
// -----------------------------------------------------------------------------
constexpr size_t TOTAL_OPS   = 1'000'000;   // total increments across all threads
constexpr int    MAX_THREADS = 32;          // you can adjust as needed
constexpr int    NET_WIDTH   = 8;

// -----------------------------------------------------------------------------
// Baseline: atomic counter
// -----------------------------------------------------------------------------
void run_atomic_counter(int num_threads) {
    std::atomic<size_t> counter{0};
    size_t ops_per_thread = TOTAL_OPS / num_threads;

    auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::thread> threads;
    threads.reserve(num_threads);

    for (int t = 0; t < num_threads; ++t) {
        threads.emplace_back([&]() {
            for (size_t i = 0; i < ops_per_thread; ++i) {
                counter.fetch_add(1, std::memory_order_relaxed);
            }
        });
    }

    for (auto& th : threads) th.join();

    auto end   = std::chrono::high_resolution_clock::now();
    double sec = std::chrono::duration<double>(end - start).count();

    double mops = (TOTAL_OPS / 1e6) / sec;
    std::cout << "[atomic] " << num_threads << " threads: "
              << mops << " Mops/s (final=" << counter.load() << ")\n";
}

// -----------------------------------------------------------------------------
// Test target: counting_net (user provides implementation)
// -----------------------------------------------------------------------------

void run_counting_net(int num_threads) {
    using counting_net = seven_jalapenos::CountingNetwork::CountingNetwork;
    counting_net counter(NET_WIDTH);
    size_t ops_per_thread = TOTAL_OPS / num_threads;

    auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::thread> threads;
    threads.reserve(num_threads);

    for (int t = 0; t < num_threads; ++t) {
        threads.emplace_back([&]() {
            for (size_t i = 0; i < ops_per_thread; ++i) {
                counter.get_and_increment(t);
            }
        });
    }

    for (auto& th : threads) th.join();

    auto end   = std::chrono::high_resolution_clock::now();
    double sec = std::chrono::duration<double>(end - start).count();

    double mops = (TOTAL_OPS / 1e6) / sec;
    std::cout << "[counting_net] " << num_threads << " threads: "
              << mops << " Mops/s \n";
}
// (final=" << counter.value() << ")
// -----------------------------------------------------------------------------
// Driver
// -----------------------------------------------------------------------------
int main() {
    for (int num_threads = 1; num_threads <= MAX_THREADS; num_threads *= 2) {
        run_atomic_counter(num_threads);
        run_counting_net(num_threads);
        std::cout << "---------------------------------\n";
    }
    return 0;
}
