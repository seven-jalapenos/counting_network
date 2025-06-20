//
// Created by jjurgenson on 25/06/19.
//

#include "hash.hpp"

#include <atomic>

namespace seven_jalapenos::HashQ {

inline int Hash::get_hash(int key) const {
    return key % length_;
}

bool Hash::put(int key, int value) {
    int h = get_hash(key);
    std::lock_guard lock(arr_mutex_[h]);
    if (!arr_[h].empty) {
        return false;
    }
    arr_[h].elt = value;
    arr_[h].empty = false;
    return true;
}

bool Hash::is_valid(int key) {
    int h = get_hash(key);
    return arr_[h].empty;
}

int Hash::get_value(int key) {
    int h = get_hash(key);
    std::lock_guard lock(arr_mutex_[h]);
    int output = arr_[h].elt;
    arr_[h].empty = true;
    return output;
}

} // seven_jalapenos::HashQ
