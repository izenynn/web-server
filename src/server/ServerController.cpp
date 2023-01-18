/** INCLUDES ----------------------------------- */

#include <server/ServerController.hpp>

/** CLASS -------------------------------------- */

namespace webserv {

const char* ServerController::k_default_path = "/etc/aps/aps.conf";

ServerController::ServerController( void ) {
	return ;
}

ServerController::~ServerController( void ) {
	if ( this->_config != webserv::nullptr_t ) {
		delete this->_config;
	}
	return ;
}

void ServerController::configLoad( const char* file ) {
	this->_config = new Config( file );
	return ;
}
void ServerController::configLoad() {
	this->configLoad( ServerController::k_default_path );
	return ;
}

void ServerController::run( void ) {
	if (this->_config == webserv::nullptr_t) {
		throw ServerController::ConfigNotLoaded();
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
			throw ServerController::ServerException("select() returned -1");
		} else if ( ret >= 0 ){
			; // TODO
		}
	}

	return ;
}

/** EXCEPTIONS --------------------------------- */

ServerController::ServerException::ServerException( const std::string& msg )
	: message(msg) {}

ServerController::ServerException::~ServerException( void ) throw () {}
const char * ServerController::ServerException::what() const throw () {
	return this->message.c_str();
}

const char * ServerController::ConfigNotLoaded::what() const throw () {
	return ( "Exception: config file not loaded" );
}

} /** namespace webserv */
