#include <iostream>

#include "nullptr_t.hpp"
#include "server/Server.hpp"

// TODO default path
int main(int argc, char * argv[]) {
	Server	*server = ft::nullptr_t;

	// to many args
	if ( argc > 2 ) {
		std::cerr << "Usage: " << argv[0] << " [CONF FILE]" << std::endl;
		return ( 1 );
	}

	// default file
	if ( argc == 1 ) {
		std::cout <<
	}

	try {
		server = new Server();
		if ( argc == 2 ) server->configLoad( argv[1] );
		else             server->configLoad(); // default path
		server->run();
		// TODO server.clean();
		delete server;
	} catch( std::exception& e ) {
		std::cout << e.what() << std::endl;
	}

	return ( 0 );
}
