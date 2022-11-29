#include <iostream>

#include "nullptr_t.hpp"
#include "server/Server.hpp"

// TODO default path
int main(int argc, char * argv[]) {
	Server	*server = ft::nullptr_t;

	if ( argc > 2 ) {
		// TODO print error / usage
		return ( 1 );
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
