/** INCLUDES ----------------------------------- */

#include <server/Client.hpp>
#include <utils/log.hpp>

/** CLASS -------------------------------------- */

namespace {
	inline void initAddr( struct sockaddr_in & addr, const webserv::listen_t & listen ) {
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = htonl(listen.host);
		addr.sin_port = htons(listen.port);
		return ;
	}
}

namespace webserv {

Client::Client( const listen_t & listen ) 
		: _fd(-1),
		  _listen(listen) {
		this->_addr.sin_family = AF_INET;
		initAddr(this->_addr, this->_listen);
		return ;
}

Client::~Client( void ) {
	return ;
}

int Client::getFd( void ) {
	return ( this->_fd );
}

void Client::start( void ) {
	this->_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_fd == -1 ) {
		log::failure( "socket() failed, could not create endpoint" );
		// TODO throw
	}

	initAddr(this->_addr, this->_listen);
	if ( -1 == bind( this->_fd, reinterpret_cast<struct sockaddr *>( &this->_addr ), sizeof( this->_addr ) ) ) {
		log::failure( "bind() failed, could not bind port" );
		// TODO throw
	}
	
	if ( -1 == listen( this->_fd, 1000 ) ) {
		log::failure( "listen() failed, could not listen" );
		// TODO throw
	}

	return ;
}

void Client::close( int socket ) {
	if ( socket > 0 ) {
		::close( socket );
	}
	// TODO clean requests of the fd
}

void Client::end( void ) {
	this->close( this->_fd );
	this->_fd = -1;
}

} /** namespace webserv */
