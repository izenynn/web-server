#include "server/Server.hpp"

const char* Server::DEFAULT_PATH = "/etc/aps/aps.conf";

Server::Server( void ) {}

void Server::configLoad( const char* file ) {
	this->_config = new Config( file );
	return ;
}
void Server::configLoad() {
	this->configLoad( Server::DEFAULT_PATH );
	return ;
}

int Server::run() {
	if (this->_config == ft::nullptr_t) {
		// throw "something went wrong, config is NULL"
		return ( 1 );
	}
	// 1. while() wait for a connection until "select" chachets something
	// 2. if ret <= 0 -> error
	// 3. else -> we are good
	//     3.1
	return ( 0 );
}

/*apsServer::apsServer() : \
	domain( AF_INET ), type( SOCK_STREAM ), protocol( 0 ), hostlong( AF_INET ), backlog( 10 ), serverFD( -1 ) {
		std::cout << C_LIGHTYELLOW << "<apsServer>: Default constructor called" << C_OFF << std::endl;
}

apsServer::apsServer( const apsServer & data ) {
	*this = data;
	std::cout << C_LIGHTYELLOW << "<apsServer>: Copy constructor called" << C_OFF << std::endl;
}

apsServer & apsServer::operator=( const apsServer & data) {
	std::cout << C_LIGHTYELLOW << "<apsServer>: Operator constructor called" << C_OFF << std::endl;
	if( this != &data ) {
		domain = data.domain;
		type = data.type;
		protocol = data.protocol;
		hostlong = data.hostlong;
		backlog = data.backlog;
		serverFD = data.serverFD;
	}
	return ( *this );
}

apsServer::~apsServer() {
	std::cout << C_LIGHTPURPLE << "<apsServer>: Destructor called" << C_OFF << std::endl;
}*/
