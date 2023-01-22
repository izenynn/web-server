/** INCLUDES ----------------------------------- */

#include <server/Server.hpp>
#include <utils/log.hpp>

/** CLASS -------------------------------------- */

namespace webserv {

const char* Server::k_default_path = "/etc/aps/aps.conf";

Server::Server( void ) 
		: _config( webserv::nullptr_t ),
		  _server_configs( webserv::nullptr_t ) {
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

	this->_server_configs = this->_config->getServers();

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

int Server::run( void ) {
	int sockfd = 0;
	std::vector<Listen *> binded;

	// setup sockets, iterate each ServerConfig
	for ( std::vector<ServerConfig *>::const_iterator it = this->_server_configs->begin(); it != this->_server_configs->end(); ++it ) {
		// set default listen if server doesnt have one
		std::vector<Listen *> & listen = (*it)->getListen();
		if ( listen.empty() ) {
			listen.push_back( new Listen("0.0.0.0", 80 ) );
		}

		// iterate listens
		for ( std::vector<Listen *>::iterator it2 = listen.begin(); it2 != listen.end(); ++it2 ) {
			// if listen not binded yet set it up
			if ( binded.end() == std::find( binded.begin(), binded.end(), *it2 ) ) {
				sockfd = socket( PF_INET, SOCK_STREAM, 0 );
				if ( -1 == sockfd ) {
					log::error( "socket() failed with return code: -1" );
					return ( -1 );
				}

				fcntl( sockfd, F_SETFL, O_NONBLOCK );

				;
			}
		}
	}

	return ( 0 );
}

} /** namespace webserv */
