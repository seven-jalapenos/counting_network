//
// Created by jjurgenson on 25/06/19.
//


#include "counting_network.hpp"

#include <iostream>

int main() {
    seven_jalapenos::CountingNetwork::CountingNetwork net(8);

    for(int i = 0; i < 10; ++i) {
        std::cout << net.get_and_increment(0);
    }

}
