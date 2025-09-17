//
// Created by jjurgenson on 25/06/19.
//

#include "hash.hpp"
#include <atomic>


namespace seven_jalapenos::HashQ {

inline size_t Hash::get_hash(size_t key) const {
    return key % length;
}

bool Hash::put(size_t key, int value) {
    size_t h = get_hash(key);
    // std::lock_guard lock(arr_mutex[h]);
    if (!arr[h].empty.load(std::memory_order_acquire) ||
        !arr[h].valid.load(std::memory_order_acquire)) {
        return false;
    }
    arr[h].elt = value;
    arr[h].empty.store(false, std::memory_order_release);
    return true;
}

bool Hash::valid_dq(size_t key) {
    size_t h = get_hash(key);
    return !arr[h].empty.load(std::memory_order_relaxed) &&
            arr[h].valid.load(std::memory_order_relaxed);
}

int Hash::get(size_t key) {
    size_t h = get_hash(key);
    // std::lock_guard lock(arr_mutex[h]);
    int output = arr[h].elt;
    arr[h].empty.store(true, std::memory_order_release);
    arr[h].valid.store(false, std::memory_order_relaxed);
    return output;
}

bool Hash::is_empty() const{
    for (auto & cell : arr){
        if(!cell.empty.load(std::memory_order_relaxed))
            return false;
    }
    return true;
}

bool Hash::is_full() const{
    for (auto & cell: arr){
        if(cell.empty.load(std::memory_order_relaxed))
            return false;
    }
    return true;
}

} // seven_jalapenos::HashQ
