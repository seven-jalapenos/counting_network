
## Repo is very messy

current queue version is **src/ring_array.hpp**  
counting network is **src/balancing_network.hpp** and **src/counting_network.hpp**  
there are a lot of other files that don't do anything

## Instructions

to run the benchmark I used run the following in terminal:  
`mkdir build && cd build && cmake .. && make`  
`cd ../test && python bench.py`  
this will run the benchmark for 1-32 threads on queues with a network of width 4, 8, and 16 and write the results to a **.csv**

alternitively, after building the binaries you can run  
`cd ../test`  
`bin/test_queue <thread_num> <counter_width> 0`  
where **<thread_num>** is the number of threads and **<counter_width>** is the width of the counter (this must be a power of two)  
this will output megaops/sec