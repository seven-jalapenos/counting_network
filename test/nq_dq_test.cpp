
#import <iostream>

#import "hash_q.hpp"

int main(){
    using namespace seven_jalapenos::HashQ;

    HashQ q(64, 8);

    for (int i = 0; i < 128; i++) {
        q.enqueue(i, 0);
        
    }
    std::cout << q.q_length << "\n";
    for (int i = 0; i < 128; i++) {
        std::cout << q.dequeue(0) << "\n";
    }

}
