/** INCLUDES ----------------------------------- */

#include <server/ServerManager.hpp>
#include <utils/log.hpp>

/** CLASS -------------------------------------- */

namespace webserv {

const char* ServerManager::k_default_path = "/etc/aps/aps.conf";

ServerManager::ServerManager( void ) 
		: _config( webserv::nullptr_t ) {
	return ;
}

ServerManager::~ServerManager( void ) {
	if ( this->_config != webserv::nullptr_t ) {
		delete this->_config;
	}
	return ;
}

void ServerManager::configLoad( const char* file ) {
	this->_config = new Config( file );
	return ;
}
void ServerManager::configLoad() {
	this->configLoad( ServerManager::k_default_path );
	return ;
}

void ServerManager::run( void ) {
	if (this->_config == webserv::nullptr_t) {
		log::error( "config file not loaded" );
		throw ServerManager::ServerManagerException("exception: config file not loaded");
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
			std::memcpy( &readfds, &this->_fd_set, sizeof( this->_fd_set ) );

			// TODO writefds must be filled with ready clients

			ret = select( this->_fd_cnt + 1, &readfds, NULL, NULL, &timeout );
		}
		if ( ret < 0 ) {
			// TODO better than exit just reset all connections and keep running
			log::error( "select() returned -1" );
			throw ServerManager::ServerManagerException( "exception: select() returned -1" );
		} else if ( ret >= 0 ){
			; // TODO
		}
	}

	return ;
}

/** EXCEPTIONS --------------------------------- */

ServerManager::ServerManagerException::ServerManagerException( const std::string & msg )
		: message( msg ) {
	return ;
}
ServerManager::ServerManagerException::~ServerManagerException( void ) throw () {
	return ;
}
const char * ServerManager::ServerManagerException::what( void ) const throw () {
	return this->message.c_str();
}

} /** namespace webserv */
