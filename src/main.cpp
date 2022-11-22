#include <iostream>

#include "config/Config.hpp"

int main(int argc, char* argv[]) {
	std::cout << "CONFIG PARSER" << std::endl;

	// TODO make "Config" class static
	if (argc != 2) return 1;
	Config config;
	try {
		config.load(std::string(argv[1]));
	} catch( std::exception& e ) {
		std::cout << e.what() << std::endl;
	}
	// .
	return 0;
}
