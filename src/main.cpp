#include <iostream>

#include <types/nullptr_t.hpp>
#include <server/Server.hpp>
#include <utils/log.hpp>

int main( int argc, char * argv[] ) {
	webserv::Server	*server = ft::nullptr_t;

	// to many args
	if ( argc > 2 ) {
		webserv::log::failure( "Error: too many arguments" );
		webserv::log::failure( "Usage: " + std::string( argv[0] ) + " [CONF FILE]" );
		return ( 1 );
	}

	// web server
	try {
		server = new webserv::Server();
		if ( argc == 2 ) server->configLoad( argv[1] );
		else             server->configLoad();
		server->run();
		// TODO server.clean();
		delete server;
	} catch( std::exception& e ) {
		std::cout << e.what() << std::endl;
	}

	return ( 0 );
}
