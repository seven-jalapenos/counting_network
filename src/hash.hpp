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
    size_t length_;
    std::vector<HashCell> arr_;
    std::vector<std::mutex> arr_mutex_;

    struct HashCell {
        int elt;
        bool empty;
        HashCell(): elt(0), empty(true) {}
    };


    explicit Hash(size_t length): length_(length), arr_(length), arr_mutex_(length){}
    bool put(int key, int value);
    int get(int key);
    int get_value(int key);
    int get_hash(int key) const;
    bool is_valid(int key);


};

class HashSegment {
public:
    explicit HashSegment(size_t length): next_(nullptr), hash_(length) {}

private:
    std::unique_ptr<HashSegment> next_;
    Hash hash_;

};


} // seven_jalapenos::HashQ

#endif //HASH_HPP
