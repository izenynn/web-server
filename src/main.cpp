#include <iostream>

#include <types/nullptr_t.hpp>
#include <server/Server.hpp>
#include <utils/log.hpp>

int main( int argc, char * argv[] ) {
	webserv::Server * manager = webserv::nullptr_t;

	// too many args
	if ( argc > 2 ) {
		webserv::log::failure( "Error: too many arguments" );
		webserv::log::failure( "Usage: " + std::string( argv[0] ) + " [CONF FILE]" );
		return ( 1 );
	}

	// help message
	if ( argc == 2 && ( 0 == strcmp( "-h", argv[1] ) || 0 == strcmp( "--help", argv[1] ) ) ) {
		webserv::log::info( "Usage: " + std::string( argv[0] ) + " [CONF FILE]" );
		return ( 0 );
	}

	// web server
	manager = new webserv::Server();

	try {
		if ( argc == 2 ) manager->load( argv[1] );
		else             manager->load();
	} catch ( std::exception & e ) {
		delete manager;
		webserv::log::error( e.what() );
		return ( 1 );
	}

	manager->run();

	delete manager;

	return ( 0 );
}
