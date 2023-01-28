/** INCLUDES ----------------------------------- */

#include <server/Server.hpp>
#include <config/constants.hpp>
#include <utils/log.hpp>
#include <utils/signals.hpp>

/** CLASS -------------------------------------- */

namespace webserv {

bool Server::_run = false;

Server::Server( void ) 
		: _config( webserv::nullptr_t ),
		  _serverConfigs( webserv::nullptr_t ),
		  _fdMax( 0 ) {
	FD_ZERO( &(this->_fdSet) );
	FD_ZERO( &(this->_fdRead) );
	FD_ZERO( &(this->_fdWrite) );
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
	for ( std::map<int, Client *>::iterator it = this->_clients.begin(); it != this->_clients.end(); ) {
		delete it->second;
		this->_clients.erase( it++ );
	}
	return ;
}

void Server::load( const char* file ) {
	this->_config = new Config();
	this->_config->load( file );

	this->_serverConfigs = this->_config->getServers();

	return ;
}
void Server::load() {
	this->load( kDefaultPath );
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

	struct timeval timeout;
	timeout.tv_sec = kTimeoutSec;
	timeout.tv_usec = 0;

	struct timespec loop_delay;
	loop_delay.tv_sec = 0;
	loop_delay.tv_nsec = kNsecLoopDelay;

	signals::handle_signals();
	this->_run = true;
	log::info( "starting server..." );

	while ( true == Server::_run ) {
		this->_fdRead	= this->_fdSet;
		this->_fdWrite	= this->_fdSet;

		ret = select( this->_fdMax + 1, &(this->_fdRead), &(this->_fdWrite), NULL, &timeout );

		if ( -1 == ret ) {
			log::failure( "select() failed with return code: -1" );
		} else if ( ret >= 0 ) {
			// iterate our listeners
			for ( std::map<int, Listen *>::const_iterator it = this->_servers.begin(); it != this->_servers.end(); ++it ) {
				// check if listener fd is on read set (if we need to read from that fd)
				if ( 0 != FD_ISSET( it->first, &(this->_fdRead) ) ) {
					// create a new client!
					this->newClient( it->first );
				}
			}

			// iterate clients to read/write
			for ( std::map<int, Client *>::const_iterator it = this->_clients.begin(); it != this->_clients.end(); ++it ) {
				int fd = it->first;
				// check read fd and receive request
				if ( FD_ISSET( fd, &(this->_fdRead) ) ) {
					int ret = this->clientRecv( fd );
					// if error reading disconnect client
					if ( 0 != ret ) {
						this->disconnectClient( fd );
						continue ;
					}
				}
				// check if timeout or need to disconnect first
				this->checkDisconnectClient( it->second );
				// check write fd and send response
				if ( FD_ISSET( fd, &(this->_fdWrite) ) ) {
					// if error sending disconnect client
					int ret = this->clientSend( fd );
					if ( 0 != ret ) {
						this->disconnectClient( fd );
						continue ;
					}
				}
			}
		} else {
			log::error( "select() failed with unexpected return code: " + SSTR( ret ) );
			return ( 1 );
		}

		while ( nanosleep( &loop_delay, &loop_delay ) );
	}

	// disconnect all clients on exit
	for ( std::map<int, Client *>::const_iterator it = this->_clients.begin(); it != this->_clients.end(); ++it ) {
		this->disconnectClient( it->first );
	}

	// disconnect all servers on exit
	for ( std::map<int, Listen *>::const_iterator it = this->_servers.begin(); it != this->_servers.end(); ++it ) {
		this->disconnectServer( it->first );
	}

	return ( 0 );
}

int Server::stop( void ) {
	Server::_run = false;
	return ( 0 );
}

int Server::clientRecv( int fd ) {
	log::warning("receiving on fd: " + SSTR( fd ) ); // DEBUG
	// get request
	Request * request = this->_clients[fd]->getRequest();
	if ( webserv::nullptr_t == request ) {
		this->_clients[fd]->initRequest();
		request = this->_clients[fd]->getRequest();
	}

	// remove fd from set
	FD_CLR( fd, &(this->_fdRead ) );

	// read socket
	//char buffer[Config::kBufferSize];
	char * buffer = reinterpret_cast<char *>( malloc( kClientMaxBodySize * sizeof( char ) ) );
	int size = recv( fd, buffer, kBufferSize, 0 );
	if ( size <= 0 ) {
		free( buffer );
		return ( 1 ); // disconnect
	}

	// convert to string
	std::string strBuffer( buffer, size );
	free( buffer );

	// parse request into request class
	log::debug( "RECV -> generating request..." );
	// TODO aqui falla
	int ret = request->parse( strBuffer );
	log::debug( "RECV -> REQUEST" );
	std::cout << "    ret: " << ret << std::endl;
	request->print(); // DEBUG
	// if error prepare response, if not, we will respond later :)
	if ( ret >= 0 ) { // FIXME we can remove this i think
	log::debug( "RECV -> generating request config..." );
		this->_clients[fd]->initRequestConfig( *(this->_serverConfigs) );
		log::debug( "RECV -> REQUEST CONFIG" );
		this->_clients[fd]->getRequestConfig()->print(); // DEBUG

	log::debug( "RECV -> generating response..." );
		this->_clients[fd]->initResponse( *(this->_serverConfigs), ret );
		log::debug( "RECV -> RESPONSE" );
		this->_clients[fd]->getResponse()->print(); // DEBUG
	}

	return ( 0 );
}
int Server::clientSend( int fd ) {
	Response * response = this->_clients[fd]->getResponse();

	// remove fd from set
	FD_CLR( fd, &(this->_fdWrite ) );

	// check response exists
	if ( webserv::nullptr_t == response ) {
		return ( 0 );
	}

	log::warning("sending on fd: " + SSTR( fd ) ); // DEBUG

	// send response
	std::cout << "\nRESPONSE:\nlenght: " << response->getResponseBody().length() << "\n" << response->getResponseBody() << std::endl; // DEBUG
	int ret = send( fd, response->getResponseBody().c_str(), response->getResponseBody().length(), 0 );

	if ( ret < 0 ) {
		log::failure( "send() failed with return code: " + SSTR( ret ) );
		return ( 1 );
	}

	bool disconnect = this->_clients[fd]->checkDisconnect() || response->isConnectionClose();
	this->_clients[fd]->clear();
	if ( true == disconnect ) return ( 1 );
	else                      return ( 0 );

	return ( 0 );
}

void Server::newClient( int fd ) {
	struct sockaddr_storage addr;
	socklen_t addr_len = sizeof( addr );

	// remove from read fd set
	FD_CLR( fd, &(this->_fdRead) );

	int sockfd = ::accept( fd, reinterpret_cast<struct sockaddr *>(&addr), &addr_len );
	if ( -1 == sockfd ) {
		log::failure( "accept() failed with return code: -1" );
		return ;
	}
	log::info( "new connection with fd: " + SSTR( sockfd ) + " on " + this->_servers[fd]->ip + ":" + SSTR( this->_servers[fd]->port ) );

	fcntl( sockfd, F_SETFL, O_NONBLOCK );

	this->_clients[sockfd] = new Client( sockfd, *(this->_servers[fd]), this->_clients.size() >= kMaxClients );

	this->addToFdSet( sockfd );

	return ;
}

void Server::disconnectClient( int fd ) {
	this->delFromFdSet( fd );

	if ( this->_clients.end() == this->_clients.find( fd ) ) {
		log::failure( "tried to close non registered connection with fd: " + SSTR( fd ) );
		return ;
	}

	delete this->_clients[fd];

	this->_clients.erase( fd );

	log::info( "closed connection with fd: " + SSTR( fd ) );

	return ;
}

void Server::checkDisconnectClient( Client * client ) {
	if ( true == client->checkTimeout() ) {
		client->initResponse( *(this->_serverConfigs), 408 ); // 408 request timeout
	}
	if ( true == client->checkDisconnect() ) {
		client->initResponse( *(this->_serverConfigs), 503 ); // 503 service unavailable
	}
}

void Server::disconnectServer( int fd ) {
	this->delFromFdSet( fd );

	if ( this->_servers.end() == this->_servers.find( fd ) ) {
		log::failure( "tried to close non registered server with fd: " + SSTR( fd ) );
		return ;
	}

	delete this->_servers[fd];
	close( fd );

	this->_servers.erase( fd );

	log::info( "closed server with fd: " + SSTR( fd ) );

	return ;
}

void Server::addToFdSet( int fd ) {
	this->_fdList.push_back( fd );
	this->_fdList.sort();

	FD_SET( fd, &(this->_fdSet) );

	if ( fd > this->_fdMax ) {
		this->_fdMax = fd;
	}

	return ;
}

void Server::delFromFdSet( int fd ) {
	for ( std::list<int>::iterator it = this->_fdList.begin(); it != this->_fdList.end(); ++it ) {
		if ( fd == *it ) this->_fdList.erase( it );
		break ;
	}

	FD_CLR( fd, &(this->_fdSet) );

	if ( fd == this->_fdMax ) {
		this->_fdMax = *(this->_fdList.rbegin() );
	}

	return ;
}

int Server::initialize( void ) {
	int sockfd = 0;
	std::vector<Listen> binded;

	// setup sockets, iterate each ServerConfig
	for ( std::vector<ServerConfig *>::const_iterator it = this->_serverConfigs->begin(); it != this->_serverConfigs->end(); ++it ) {
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

				struct timespec loop_delay;
				loop_delay.tv_sec = 10;
				loop_delay.tv_nsec = 0;
				bool isBinded = false;
				for ( int tries = 0; tries < 10; ++tries ) {
					if ( -1 == bind( sockfd, reinterpret_cast<struct sockaddr *>(&addr), sizeof( addr ) ) ) {
						log::failure( "bind() for address " + (*it2)->ip + ":" + SSTR( (*it2)->port ) + " failed with return code: -1, retrying..." );
					} else {
						isBinded = true;
						break ;
					}
					while ( nanosleep( &loop_delay, &loop_delay ) );
				}
				if ( false == isBinded ) {
					log::error( "bind() for address " + (*it2)->ip + ":" + SSTR( (*it2)->port ) + " failed with return code: -1" );
					return ( -1 );
				}

				//int option_value = 1;
				//setsockopt( sockfd, SOL_SOCKET, SO_REUSEADDR, &option_value, sizeof( int ));

				if ( -1 == listen( sockfd, kBacklogSize ) ) {
					log::error( "listen() for address " + (*it2)->ip + ":" + SSTR( (*it2)->port ) + " failed with return code: -1" );
					return ( -1 );
				}

				this->_servers[sockfd] = new Listen( (*it2)->ip, (*it2)->port );
				this->addToFdSet( sockfd );
				binded.push_back( **it2 );

				log::info( "started listening on " + (*it2)->ip + ":" + SSTR( (*it2)->port ) + " with fd: " + SSTR( sockfd ) );
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
