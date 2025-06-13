
#include <vector>
#include <iostream>

#include "../src/bitonic_network.hpp"

int main(){
	using std::vector;
    BitonicNetwork<std::vector<int>> net(8);

	auto vec_to_string = [](const std::vector<int>& vec)->std::string{ // test
	    using std::string;
	    string output;
	    for(int i : vec){
	        output += i + " ";
	    }
	    return output;
	};

	
	for (int i = 0; i < 64; i++){
		auto q = net.traverse(1);
		q->push_back(i);
	}

	for (int i = 0; i < net.width(); ++i) {
		std::string str = vec_to_string(*net.get_elt(i));
		std::cout << str << "\n";
	}
    return 0;
}
