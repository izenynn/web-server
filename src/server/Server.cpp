/** INCLUDES ----------------------------------- */

#include "server/Server.hpp"

/** CLASS -------------------------------------- */

namespace webserv {

const char* Server::DEFAULT_PATH = "/etc/aps/aps.conf";

Server::Server( void ) {
	return ;
}

Server::~Server( void ) {
	if ( this->_config != ft::nullptr_t ) {
		delete this->_config;
	}
	return ;
}

void Server::configLoad( const char* file ) {
	this->_config = new Config( file );
	return ;
}
void Server::configLoad() {
	this->configLoad( Server::DEFAULT_PATH );
	return ;
}

void Server::run() {
	if (this->_config == ft::nullptr_t) {
		throw Server::ConfigNotLoaded();
		return ;
	}
	// TODO receive connections
	// 1. while() wait for a connection until "select" chachets something
	// 2. if ret <= 0 -> error
	// 3. else -> we are good
	//     3.1
	return ;
}

const char* Server::ConfigNotLoaded::what() const throw() {
	return ( "Exception: config file not loaded" );
}

} // namespace webserv
