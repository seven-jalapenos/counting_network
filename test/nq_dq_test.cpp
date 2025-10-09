
#include <iostream>
#include <thread>
#include <vector>

#include "ttq.hpp"
#include <boost/lockfree/queue.hpp>

constexpr int MILLION = 1'000'000;

double net_bm(int num_threads, int width, int warm_up=3, int runs=10){
    using namespace seven_jalapenos::TTQ;
    // std::mutex io_mutex;
    std::vector<int> per_thread_reps(num_threads);
    int iter_per_thread = (MILLION / 2) / num_threads;
    int extra_iter = (MILLION / 2) % num_threads;

    auto even_mixed_op = [num_threads](TTQ *q, int id, int rep){
        // enqueue
        for (int i = 0; i < rep; i++) {
            q->enqueue(id + i * num_threads, id);
        }
        // dequeue
        for (int i = 0; i < rep; i++) {
            q->dequeue(id);
        }
    };

    for (int i = 0; i < warm_up; i++) {
        TTQ q(width, MILLION);
        std::vector<std::thread> threads;

        for (int ii = 0; ii < num_threads; ii++) {
            int iters  = ii < extra_iter ? iter_per_thread + 1 : iter_per_thread;
            per_thread_reps[ii] = iters;
            threads.emplace_back(even_mixed_op, &q, num_threads, iters);
        }
        for (auto & t : threads) {
            t.join();
            // std::cout << std::format("thread {} has joined", std::hash<std::thread::id>{}(std::this_thread::get_id()));
        }
    }

    using namespace std::chrono;
    duration<double> total_time;
    for (int i = 0; i < runs; i++) {
        TTQ q(width, MILLION);
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

double boost_bm(int num_threads, int warm_up=3, int runs=10){
    using namespace boost::lockfree;
    std::vector<int> per_thread_reps(num_threads);
    int iter_per_thread = (MILLION / 2) / num_threads;
    int extra_iter = (MILLION / 2) % num_threads;

    auto even_mixed_op_boost = [num_threads](queue<int> *q, int id, int rep){
        // enqueue
        for (int i = 0; i < rep; i++) {
            q->push(id + i * num_threads);
        }
        // dequeue
        int value;
        for (int i = 0; i < rep; i++) {
            q->pop(value);
        }
    };

    // boost version
    for (int i = 0; i < warm_up; i++) {
        queue<int> q(MILLION);
        std::vector<std::thread> threads;

        for (int ii = 0; ii < num_threads; ii++) {
            int iters  = ii < extra_iter ? iter_per_thread + 1 : iter_per_thread;
            per_thread_reps[ii] = iters;
            threads.emplace_back(even_mixed_op_boost, &q, num_threads, iters);
        }
        for (auto & t : threads) {
            t.join();
            // std::cout << std::format("thread {} has joined", std::hash<std::thread::id>{}(std::this_thread::get_id()));
        }
    }

    using namespace std::chrono;
    duration<double> total_time_boost;
    for (int i = 0; i < runs; i++) {
        queue<int> q(MILLION);
        std::vector<std::thread> threads;

        auto start = high_resolution_clock::now();
        for (int ii = 0; ii < num_threads; ii++) {
            threads.emplace_back(even_mixed_op_boost, &q, num_threads, per_thread_reps[ii]);
        }
        for (auto & t : threads) {
            t.join();
        }
        auto end = high_resolution_clock::now();
        total_time_boost += end - start;

    }
    
    double avg_time_boost = total_time_boost.count() / runs;
    double megaops_per_sec_boost = 1 / avg_time_boost;
    return megaops_per_sec_boost;
}

int main(int argc, char* argv[]){
    // int length = 32;
    int width = 8;
    int num_threads = 32;
    int runs = 10;
    int warm_up = 3;
    int wait = 0;
    int which = 2;
    switch (argc) {
    case 4:
        which = std::stoi(argv[3]);
        [[fallthrough]];
    case 3:
        width = std::stoi(argv[2]);
        [[fallthrough]];
    case 2:
        num_threads = std::stoi(argv[1]);
        [[fallthrough]];
    case 1:
        break;
    default:
        throw(std::runtime_error("invalid number of inline arguments"));
    }

    double megaops_per_sec_ttq = 0, megaops_per_sec_boost = 0;
    bool ttq = false; 
    switch (which){
    case 0:
        ttq = true;
        break;
    case 1:
        break;
    }

    if (ttq){
        megaops_per_sec_ttq = net_bm(num_threads, width);
        std::cout << "megaops per second (enqueue & dequeue): " << megaops_per_sec_ttq << "\n";
    }
    if (!ttq){
        megaops_per_sec_boost = boost_bm(num_threads);
        std::cout << "megaops per second (enqueue & dequeue) boost: " << megaops_per_sec_boost << "\n";
    }

    std::cout << (!ttq ? std::to_string(megaops_per_sec_boost) : "")
              << (ttq ? std::to_string(megaops_per_sec_ttq) : "")
              << std::endl;
}
