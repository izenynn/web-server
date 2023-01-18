#include <iostream>

#include <types/nullptr_t.hpp>
#include <server/ServerController.hpp>
#include <utils/log.hpp>

int main( int argc, char * argv[] ) {
	webserv::ServerController *controller = webserv::nullptr_t;

	// to many args
	if ( argc > 2 ) {
		webserv::log::failure( "Error: too many arguments" );
		webserv::log::failure( "Usage: " + std::string( argv[0] ) + " [CONF FILE]" );
		return ( 1 );
	}

	// web server
	try {
		controller = new webserv::ServerController();
		if ( argc == 2 ) controller->configLoad( argv[1] );
		else             controller->configLoad();
		controller->run();
		// TODO server.clean();
		delete controller;
	} catch( std::exception& e ) {
		std::cout << e.what() << std::endl;
	}

	return ( 0 );
}
