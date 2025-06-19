
#include "counting_network.hpp"

#include <vector>
#include <iostream>
#include <sstream>

#include "ordering_network.hpp"

int main(){
	using std::vector;
	using std::string;
	using seven_jalapenos::CountingNetwork::OrderingNetwork;
    OrderingNetwork<vector<int>> net(8);

	auto vec_to_string = [](const std::vector<int>& vec)->std::string{ // test
	    std::ostringstream output;
	    for(int i : vec){
	        output << i << " ";
	    }
	    return output.str();
	};

	
	for (int i = 0; i < 16; i++){
		auto q = net.traverse(1);
		q->push_back(i);
	}

	for (int i = 0; i < net.width(); ++i) {
		string str = vec_to_string(*net.get_elt(i));
		std::cout << str << "\n";
	}
    return 0;
}
