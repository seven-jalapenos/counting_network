//
// Created by jjurgenson on 25/06/19.
//

#ifndef HASH_HPP
#define HASH_HPP
#include <cstddef>
#include <memory>
#include <vector>
#include <atomic>

namespace seven_jalapenos::HashQ {

class Hash {
public:
    struct alignas(64) HashCell {
        int elt;
        std::atomic<bool> empty;
        std::atomic<bool> valid;
        HashCell(): elt(0), empty(true), valid(true) {}
    };

    size_t const length;
    std::vector<HashCell> arr;
    // std::vector<std::mutex> arr_mutex;

    explicit Hash(size_t length): length(length), arr(length) {}
    bool put(size_t key, int value);
    int get(size_t key); // removes and returns value
    size_t get_hash(size_t key) const;
    bool valid_dq(size_t key);
    bool is_empty() const;
    bool is_full() const;
};

class HashSegment {
public:
    std::atomic<bool> enqueue_phase;
    // std::shared_mutex nq_mtx;
    // std::shared_mutex dq_mtx;
    std::unique_ptr<HashSegment> next;
    Hash hash;

    explicit HashSegment(size_t length): enqueue_phase(true), next(nullptr), hash(length) {}
    // void set_phase_enqueue();
    // void set_phase_dequeue();
};


} // seven_jalapenos::HashQ

#endif //HASH_HPP
