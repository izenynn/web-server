/** INCLUDES ----------------------------------- */

#include <server/Server.hpp>

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

void Server::run( void ) {
	if (this->_config == ft::nullptr_t) {
		throw Server::ConfigNotLoaded();
		return ;
	}

	fd_set			readfds;
	struct timeval	timeout;
	int				ret;

	timeout.tv_sec = 5; timeout.tv_usec = 0;
	return ; // TODO temporary return to avoid hang up on this point until webserv is completed
	while ( 1 ) {
		ret = 0;

		while ( ret == 0 ) {
			std::memcpy(&readfds, &this->_fd_set, sizeof(this->_fd_set));

			// TODO writefds must be filled with ready clients

			ret = select(this->_fd_cnt + 1, &readfds, NULL, NULL, &timeout);
		}
		if ( ret < 0 ) {
			// TODO better than exit just reset all connections and keep running
			throw Server::ServerException("select() returned -1");
		} else if ( ret >= 0 ){
			; // TODO
		}
	}

	return ;
}

/** EXCEPTIONS --------------------------------- */

Server::ServerException::ServerException( const std::string& msg )
	: message(msg) {}
Server::ServerException::~ServerException( void ) throw () {}
const char * Server::ServerException::what() const throw () {
	return this->message.c_str();
}

const char * Server::ConfigNotLoaded::what() const throw () {
	return ( "Exception: config file not loaded" );
}

} /** namespace webserv */
