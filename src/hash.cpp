//
// Created by jjurgenson on 25/06/19.
//

#include "hash.hpp"

// #include <atomic>

namespace seven_jalapenos::HashQ {

inline int Hash::get_hash(int key) const {
    return key % length;
}

bool Hash::put(int key, int value) {
    int h = get_hash(key);
    std::lock_guard lock(arr_mutex[h]);
    if (!arr[h].empty) {
        return false;
    }
    arr[h].elt = value;
    arr[h].empty = false;
    return true;
}

bool Hash::valid_dq(int key) {
    int h = get_hash(key);
    return !arr[h].empty;
}

int Hash::get_elt(int key) {
    int h = get_hash(key);
    std::lock_guard lock(arr_mutex[h]);
    int output = arr[h].elt;
    arr[h].empty = true;
    return output;
}

bool Hash::is_empty() const{
    for (auto & cell : arr){
        if(!cell.empty)
            return false;
    }
    return true;
}

bool Hash::is_full() const{
    for (auto & cell: arr){
        if(cell.empty)
            return false;
    }
    return true;
}

} // seven_jalapenos::HashQ
