
#include "counting_network.hpp"

#include <vector>
#include <iostream>
#include <sstream>

#include "balancing_network.hpp"

int main(){
	using std::vector;
	using std::string;
	using seven_jalapenos::CountingNetwork::BalancingNetwork;
	int width = 8;
    BalancingNetwork net(width);
	vector<vector<int>> v(width);

	auto vec_to_string = [](const std::vector<int>& vec)->std::string{ // test
	    std::ostringstream output;
	    for(int i : vec){
	        output << i << " ";
	    }
	    return output.str();
	};

	
	for (int i = 0; i < 16; i++){
		int q = net.traverse(1);
		v[q].push_back(i);
	}

	for (int i = 0; i < width; ++i) {
		string str = vec_to_string(v[i]);
		std::cout << str << "\n";
	}
    return 0;
}
