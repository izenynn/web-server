#include "Server.hpp"
#include "Print.hpp"

/*
====================
Constructors & Destructors
Automatically called when an object of the class Server is created or deleted
====================
*/
// @DUDAS: He visto ejemplos de backlog usados en 200. He puesto 10 porque creo que entre 5 y 10 es lo correcto, pero podríamos tener que ampliarlo.
apsServer::apsServer() : \
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
}