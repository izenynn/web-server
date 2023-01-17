/** INCLUDES ----------------------------------- */

#include <server/Client.hpp>
#include <sys/socket.h>
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

const size_t Client::k_recv_size = 4096;

Client::Client( const listen_t & listen ) 
		: _sockfd(-1),
		  _host(listen) {
		this->_addr.sin_family = AF_INET;
		initAddr(this->_addr, this->_host);
		return ;
}

Client::~Client( void ) {
	return ;
}

int Client::getFd( void ) {
	return ( this->_sockfd );
}

void Client::start( void ) {
	this->_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_sockfd == -1 ) {
		log::failure( "socket() failed, could not create endpoint" );
		// TODO throw / log error ???
	}

	initAddr(this->_addr, this->_host);
	if ( -1 == bind( this->_sockfd, reinterpret_cast<struct sockaddr *>( &this->_addr ), sizeof( this->_addr ) ) ) {
		log::failure( "bind() failed, could not bind port" );
		// TODO throw / log error ???
	}
	
	if ( -1 == listen( this->_sockfd, 1000 ) ) {
		log::failure( "listen() failed, could not listen" );
		// TODO throw / log error ???
	}

	return ;
}

void Client::end( void ) {
	this->close( this->_sockfd );
	this->_sockfd = -1;
	return ;
}

int Client::accept( void ) {
	int socket = ::accept(this->_sockfd, NULL, NULL);
	if ( -1 == socket ) {
		log::failure( "accept() failed, could not create socket" );
		return ( -1 );
		
	}
	fcntl(socket, F_SETFL, O_NONBLOCK);
	// TODO initialize requests ?
	return ( socket );
}

int Client::send( int socket ) {
	static std::map<int, size_t> sent;

	// create map entry if doesnt exists
	if ( sent.end() == sent.find( socket ) ) {
		sent[socket] = 0;
	}

	// TODO
	if ( 0 == sent[socket] ) {
		// TODO
	}
	// TODO

	return ( 0 );
}

int Client::recv( int socket ) {
	char	buffer[Client::k_recv_size] = {};
	int		ret;

	ret = ::recv( socket, buffer, Client::k_recv_size - 1, 0 );
	if ( -1 == ret ) {
		log::info( "socket: " + SSTR( socket ) + ": recv() error, connection closed" );
		this->close( socket );
		return ( -1 );
	}
	if ( 0 == ret ) {
		log::info( "socket: " + SSTR( socket ) + ": connection was closed by a client" );
		this->close( socket );
		return ( -1 );
	}

	// TODO read and save request

	return ( 0 );
}

void Client::close( int socket ) {
	if ( socket > 0 ) {
		::close( socket );
	}
	// TODO clean requests of the fd
	return ;
}

} /** namespace webserv */
