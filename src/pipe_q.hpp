
#ifndef PIPE_Q_HPP
#define PIPE_Q_HPP

#import "basic_q.hpp";
#import "counting_network.hpp";

namespace seven_jalapenos::PipeQ{

class PipeQ{
public:
    int const width;

    PipeQ(int width);
    void enqueue(int value);
    int dequeue();

private:
    CountingNetwork::CountingNetwork nq_net_;
    CountingNetwork::CountingNetwork dq_net_;
    std::vector<BasicQueue<int>> pipes_;

};

} // seven_jalapenos::PipeQ



#endif // PIPE_Q_HPP