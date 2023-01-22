#include <cstring> // std::strcmp()

#include <types/nullptr_t.hpp>
#include <server/Server.hpp>
#include <utils/log.hpp>

int main( int argc, char * argv[] ) {
	int					ret = 0;
	webserv::Server *	server = webserv::nullptr_t;

	// too many args
	if ( argc > 2 ) {
		webserv::log::failure( "Error: too many arguments" );
		webserv::log::failure( "Usage: " + std::string( argv[0] ) + " [CONF FILE]" );
		return ( 1 );
	}

	// help message
	if ( argc == 2 && ( 0 == std::strcmp( "-h", argv[1] ) || 0 == std::strcmp( "--help", argv[1] ) ) ) {
		webserv::log::info( "Usage: " + std::string( argv[0] ) + " [CONF FILE]" );
		return ( 0 );
	}

	// web server
	server = new webserv::Server();

	try {
		if ( argc == 2 ) server->load( argv[1] );
		else             server->load();
	} catch ( std::exception & e ) {
		delete server;
		webserv::log::error( e.what() );
		return ( 1 );
	}

	//server->print();

	ret = server->start();

	delete server;

	return ( ret );
}
