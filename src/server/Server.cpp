/** INCLUDES ----------------------------------- */

#include <server/Server.hpp>
#include <utils/log.hpp>

/** CLASS -------------------------------------- */

namespace webserv {

const char* Server::k_default_path = "/etc/aps/aps.conf";

Server::Server( void ) 
		: _config( webserv::nullptr_t ),
		  _servers_config( webserv::nullptr_t ) {
	return ;
}

Server::~Server( void ) {
	if ( this->_config != webserv::nullptr_t ) {
		delete this->_config;
	}
	return ;
}

void Server::load( const char* file ) {
	this->_config = new Config();
	this->_config->load( file );

	this->_servers_config = this->_config->getServers();

	return ;
}
void Server::load() {
	this->load( Server::k_default_path );
	return ;
}

void Server::print( void ) {
	this->_config->print();
	return ;
}

void Server::run( void ) {
	return ; // FIXME temporary return to avoid crash :"D
	if (this->_config == webserv::nullptr_t) {
		//throw Server::ServerManagerException("exception: config file not loaded");
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
			//std::memcpy( &readfds, &this->_fd_set, sizeof( this->_fd_set ) );

			// TODO writefds must be filled with ready clients

			ret = select( this->_fd_cnt + 1, &readfds, NULL, NULL, &timeout );
		}
		if ( ret < 0 ) {
			// TODO better than exit just reset all connections and keep running
			//throw ServerManager::ServerManagerException( "exception: select() returned -1" );
		} else if ( ret >= 0 ){
			; // TODO
		}
	}

	return ;
}

} /** namespace webserv */
