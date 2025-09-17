
#include "pipe_q.hpp"
#include "basic_q.hpp"
#include "counting_network.hpp"

namespace seven_jalapenos::PipeQ{

PipeQ::PipeQ(int width): width(width){
    CountingNetwork::CountingNetwork nq_net_(width);    
    CountingNetwork::CountingNetwork dq_net_(width);
    std::vector<BasicQueue<int>> pipes_(width);
    
    for (int i = 0; i < width; i++) {-
        pipes_.push_back(BasicQueue<int>());
    }
}


} // seven_jalapenos::pipeQ