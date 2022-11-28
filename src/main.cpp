#include <iostream>

#include "config/Config.hpp"

int main(int argc, char * argv[]) {
	if ( argc != 2 ) {
		// TODO print error
		return ( 1 );
	}

	// TODO make "Config" class static
	if (argc != 2) return 1;
	Config config;
	try {
		config.load(std::string(argv[1])); // TODO server.configLoad()
		// TODO server.run()
		// TODO server.clean()
	} catch( std::exception& e ) {
		std::cout << e.what() << std::endl;
	}

	return ( 0 );
}
