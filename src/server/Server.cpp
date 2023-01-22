/** INCLUDES ----------------------------------- */

#include <server/Server.hpp>
#include <utils/log.hpp>

/** CLASS -------------------------------------- */

namespace webserv {

const char *	Server::k_default_path = "/etc/aps/aps.conf";
const int		Server::k_backlog_size = 1024;
const int		Server::k_max_clients = 1024;
const int		Server::k_buffer_size = 16384;
const __time_t	Server::k_timeout_sec = 5;

Server::Server( void ) 
		: _config( webserv::nullptr_t ),
		  _server_configs( webserv::nullptr_t ) {
	FD_ZERO( &(this->_fd_set) );
	FD_ZERO( &(this->_fd_read) );
	FD_ZERO( &(this->_fd_write) );
	return ;
}

Server::~Server( void ) {
	if ( this->_config != webserv::nullptr_t ) {
		delete this->_config;
	}
	for ( std::map<int, Listen *>::iterator it = this->_servers.begin(); it != this->_servers.end(); ) {
		delete it->second;
		this->_servers.erase( it++ );
	}
	/*for ( std::map<int, Clients *>::iterator it = this->_clients.begin(); it != this->_clients.end(); ) {
		delete it->second;
		this->_clients.erase( it++ );
	}*/
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

int Server::start( void ) {
	int ret = 0;

	ret = this->initialize();
	if ( 0 != ret ) {
		return ( ret );
	}

	log::info( "starting server..." );

	struct timeval timeout;
	timeout.tv_sec = this->k_timeout_sec;
	timeout.tv_usec = 0;
	//(void)timeout;

	while ( true ) {
		this->_fd_read	= this->_fd_set;
		this->_fd_write	= this->_fd_set;

		ret = select( this->_fd_max + 1, &(this->_fd_read), &(this->_fd_write), NULL, &timeout );

		// TODO stuff
	}

	return ( 0 );
}

void Server::addToFdSet( int fd ) {
	this->_fd_list.push_back( fd );
	this->_fd_list.sort();

	FD_SET( fd, &(this->_fd_set) );

	if ( fd > this->_fd_max ) {
		this->_fd_max = fd;
	}
}

void Server::delFromFdSet( int fd ) {
	for ( std::list<int>::iterator it = this->_fd_list.begin(); it != this->_fd_list.end(); ++it ) {
		if ( fd == *it ) this->_fd_list.erase( it );
		break ;
	}

	FD_CLR( fd, &(this->_fd_set) );

	if ( fd == this->_fd_max ) {
		this->_fd_max = *(this->_fd_list.rbegin() );
	}
}

int Server::initialize( void ) {
	int sockfd = 0;
	std::vector<Listen> binded;

	// setup sockets, iterate each ServerConfig
	for ( std::vector<ServerConfig *>::const_iterator it = this->_server_configs->begin(); it != this->_server_configs->end(); ++it ) {
		// set default listen if server doesnt have one
		std::vector<Listen *> & listens = (*it)->getListen();
		if ( true == listens.empty() ) {
			listens.push_back( new Listen("0.0.0.0", 80 ) );
		}

		// iterate listens
		for ( std::vector<Listen *>::iterator it2 = listens.begin(); it2 != listens.end(); ++it2 ) {
			// if listen not binded yet set it up
			if ( binded.end() == std::find( binded.begin(), binded.end(), **it2 ) ) {
				sockfd = socket( PF_INET, SOCK_STREAM, 0 );
				if ( -1 == sockfd ) {
					log::error( "socket() failed with return code: -1" );
					return ( -1 );
				}

				fcntl( sockfd, F_SETFL, O_NONBLOCK );

				struct sockaddr_in addr;
				addr.sin_family = AF_INET;
				addr.sin_addr.s_addr = inet_addr( (*it2)->ip.c_str() );
				addr.sin_port = htons( (*it2)->port );
				if ( -1 == bind( sockfd, reinterpret_cast<struct sockaddr *>( &addr ), sizeof( addr ) ) ) {
					log::error( "bind() for address " + (*it2)->ip + ":" + SSTR( (*it2)->port ) + " failed with return code: -1" );
					return ( -1 );
				}

				//int option_value = 1;
				//setsockopt( sockfd, SOL_SOCKET, SO_REUSEADDR, &option_value, sizeof( int ));

				if ( -1 == listen( sockfd, this->k_backlog_size ) ) {
					log::error( "listen() for address " + (*it2)->ip + ":" + SSTR( (*it2)->port ) + " failed with return code: -1" );
					return ( -1 );
				}

				this->_servers[sockfd] = new Listen( (*it2)->ip, (*it2)->port );
				this->addToFdSet( sockfd );
				binded.push_back( **it2 );

				log::info( "started listening on " + (*it2)->ip + ":" + SSTR( (*it2)->port ) );
			}
		}
	}
	if ( true == binded.empty() ) {
		log::error( "failed to start server" );
		return ( -1 );
	}

	log::success( "server initialition successful" );

	return ( 0 );
}

} /** namespace webserv */
