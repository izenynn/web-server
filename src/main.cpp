#include <iostream>

#include <types/nullptr_t.hpp>
#include <server/ServerManager.hpp>
#include <utils/log.hpp>

int main( int argc, char * argv[] ) {
	webserv::ServerManager * manager = webserv::nullptr_t;

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
	manager = new webserv::ServerManager();

	try {
		if ( argc == 2 ) manager->configLoad( argv[1] );
		else             manager->configLoad();
	} catch( std::exception& e ) {
		std::cout << e.what() << std::endl;
		delete manager;
		return ( 1 );
	}

	//manager->run();

	// TODO server.clean();
	delete manager;

	return ( 0 );
}
