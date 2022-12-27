#include <iostream>

#include <types/nullptr_t.hpp>
#include <server/Server.hpp>
#include <utils/log.hpp>

int main(int argc, char * argv[]) {
	Server	*server = ft::nullptr_t;

	// to many args
	if ( argc > 2 ) {
		std::cerr << "Usage: " << argv[0] << " [CONF FILE]" << std::endl;
		webserv::log::info("info");
		webserv::log::success("success");
		webserv::log::warning("warning");
		webserv::log::failure("failure");
		webserv::log::error("error");
		return ( 1 );
	}

	// default conf file
	if ( argc == 1 ) {
		webserv::log::info("TODO");
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
