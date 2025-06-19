//
// Created by jjurgenson on 25/06/19.
//

#ifndef HASH_HPP
#define HASH_HPP

namespace seven_jalapenos::HashQ {

class hash {
public:

    hash(size_t length): length_(length) {}
    void put(int key, int value) {}
    int get_value(int key);


private:
    size_t length_;
    int arr[];

    int get_hash(int key);

};

} // seven_jalapenos::HashQ

#endif //HASH_HPP
