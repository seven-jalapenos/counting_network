//
// Created by jjurgenson on 25/06/19.
//

#ifndef HASH_HPP
#define HASH_HPP
#include <memory>
#include <vector>

namespace seven_jalapenos::HashQ {

class Hash {
public:
    struct alignas(64) HashCell {
        int elt;
        bool empty;
        HashCell(): elt(0), empty(true) {}
    };

    size_t const length;
    std::vector<HashCell> arr;
    std::vector<std::mutex> arr_mutex;

    explicit Hash(size_t length): length(length), arr(length), arr_mutex(length){}
    bool put(int key, int value);
    int get_elt(int key);
    int get_hash(int key) const;
    bool valid_dq(int key);
    bool is_empty() const;
    bool is_full() const;
};

class HashSegment {
public:
    std::unique_ptr<HashSegment> next;
    Hash hash;

    explicit HashSegment(size_t length): next(nullptr), hash(length) {}
};


} // seven_jalapenos::HashQ

#endif //HASH_HPP
