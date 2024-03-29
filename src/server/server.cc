/** INCLUDES ----------------------------------- */

#include "server/server.h"

#include <sys/types.h>
#include <sys/socket.h> // socket()
#include <sys/select.h> // select()
#include <fcntl.h> // fcntl()
#include <netinet/in.h> // sockaddr_in
#include <arpa/inet.h> // inet_addr()

#include <algorithm> // std::find()
#include <ctime> // nanosleep()

#include "nstd/memory.h"

#include "webserv.h"
#include "utils/signals.h"

#include "config/server_config.h"
#include "server/listen.h"
#include "request/request.h"
#include "request/request_data.h"
#include "response/response.h"

/** CLASS -------------------------------------- */

namespace webserv {

bool Server::_run = false;

Server::Server( void ) 
    : _config( nullptr ),
      _serverConfigs( nullptr ),
      _fdMax( 0 ) {
  FD_ZERO( &(this->_fdSet) );
  FD_ZERO( &(this->_fdRead) );
  FD_ZERO( &(this->_fdWrite) );
  return ;
}

Server::~Server( void ) {
  if ( this->_config != nullptr ) {
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
  LOG_INFO( "starting server..." );

  while ( true == Server::_run ) {
    this->_fdRead = this->_fdSet;
    this->_fdWrite  = this->_fdSet;

    ret = select( this->_fdMax + 1, &( this->_fdRead ), &( this->_fdWrite ), NULL, &timeout );

    if ( -1 == ret && true == Server::_run ) {
      LOG_FAILURE( "select() failed with return code: -1" );
    } else if ( ret >= 0 ) {
      // iterate our listeners
      for ( std::map<int, Listen *>::const_iterator it = this->_servers.begin(); it != this->_servers.end(); ++it ) {
        // check if listener fd is on read set (if we need to read from that fd)
        if ( 0 != FD_ISSET( it->first, &( this->_fdRead ) ) ) {
          // create a new client!
          this->newClient( it->first );
        }
      }

      // iterate clients to read/write
      for ( std::map<int, Client *>::const_iterator it = this->_clients.begin(); it != this->_clients.end(); ) {
        int fd = it->first;
        // check read fd and receive request
        if ( FD_ISSET( fd, &( this->_fdRead ) ) ) {
          int ret_aux = this->clientRecv( fd );
          // if error reading disconnect client
          if ( 0 != ret_aux ) {
            ++it;
            this->disconnectClient( fd );
            continue ;
          }
        }
        // check if timeout or need to disconnect first
        this->checkDisconnectClient( it->second );
        // check write fd and send response
        if ( FD_ISSET( fd, &(this->_fdWrite) ) ) {
          // if error sending disconnect client
          int ret_aux = this->clientSend( fd );
          if ( 0 != ret_aux ) {
            ++it;
            this->disconnectClient( fd );
            continue ;
          }
        }
        ++it;
      }
    } else if ( true == Server::_run ){
      LOG_ERROR( "select() failed with unexpected return code: " << ret );
      return ( 1 );
    }

    while ( nanosleep( &loop_delay, &loop_delay ) );
  }

  LOG_INFO( "stoping server..." );

  // disconnect all clients on exit
  for ( std::map<int, Client *>::const_iterator it = this->_clients.begin(); it != this->_clients.end(); ) {
    this->disconnectClient( (it++)->first );
  }

  // disconnect all servers on exit
  for ( std::map<int, Listen *>::const_iterator it = this->_servers.begin(); it != this->_servers.end(); ) {
    this->disconnectServer( (it++)->first );
  }

  LOG_SUCCESS( "server stop successful" );

  return ( 0 );
}

int Server::stop( void ) {
  Server::_run = false;
  return ( 0 );
}

int Server::clientRecv( int fd ) {
  // get request
  Request * request = this->_clients[fd]->getRequest();
  if ( nullptr == request ) {
    this->_clients[fd]->initRequest();
    request = this->_clients[fd]->getRequest();
  }

  // remove fd from set
  FD_CLR( fd, &(this->_fdRead ) );

  // read socket
  nstd::unique_ptr<char[]> buffer( new char[kBufferSize * sizeof( char )] );
  ssize_t size = recv( fd, buffer.get(), kBufferSize, 0 );
  if ( size <= 0 ) {
    return ( 1 ); // disconnect
  }

  // convert to string
  std::string strBuffer( buffer.get(), static_cast<std::string::size_type>( size ) );

  // parse request into request class
  int status_code = request->parse( strBuffer );
  this->_clients[fd]->initRequestData( *(this->_serverConfigs) );
  this->_clients[fd]->initResponse( *(this->_serverConfigs), status_code );

  return ( 0 );
}

int Server::clientSend( int fd ) {
  Response * response = this->_clients[fd]->getResponse();

  // remove fd from set
  FD_CLR( fd, &(this->_fdWrite ) );

  // check response exists
  if ( nullptr == response ) {
    return ( 0 );
  }

  // send response
  ssize_t ret = send( fd, response->getResponseBody().c_str(), response->getResponseBody().length(), 0 );

  if ( ret < 0 ) {
    LOG_FAILURE( "send() failed with return code: " << ret );
    return ( 1 );
  }

  bool disconnect = this->_clients[fd]->getDisconnect() || response->isConnectionClose();
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
    LOG_FAILURE( "accept() failed with return code: -1" );
    return ;
  }

  fcntl( sockfd, F_SETFL, O_NONBLOCK );

  // create client
  this->_clients[sockfd] = new Client( sockfd, *(this->_servers[fd]) );
  // disconnect if max clients
  if ( this->_clients.size() - 1 >= kMaxClients ) {
    this->_clients[sockfd]->setDisconnct( true );
  }

  this->addToFdSet( sockfd );

  return ;
}

void Server::disconnectClient( int fd ) {
  this->delFromFdSet( fd );

  if ( this->_clients.end() == this->_clients.find( fd ) ) {
    LOG_FAILURE( "tried to close non registered connection with fd: " << fd );
    return ;
  }

  delete this->_clients[fd];

  this->_clients.erase( fd );

  return ;
}

void Server::checkDisconnectClient( Client * client ) {
  if ( true == client->getDisconnect() ) {
    client->initResponse( *(this->_serverConfigs), 503 ); // 503 service unavailable
  }
}

void Server::disconnectServer( int fd ) {
  this->delFromFdSet( fd );

  if ( this->_servers.end() == this->_servers.find( fd ) ) {
    LOG_FAILURE( "tried to close non registered server with fd: " << fd );
    return ;
  }

  delete this->_servers[fd];
  close( fd );

  this->_servers.erase( fd );

  LOG_INFO( "closed server with fd: " << fd );

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
    if ( fd == *it ) {
      this->_fdList.erase( it );
      break ;
    }
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
          LOG_ERROR( "socket() failed with return code: -1" );
          return ( -1 );
        }

        fcntl( sockfd, F_SETFL, O_NONBLOCK );

        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = inet_addr( (*it2)->_ip.c_str() );
        addr.sin_port = htons( (*it2)->_port );

        struct timespec loop_delay;
        loop_delay.tv_sec = 10;
        loop_delay.tv_nsec = 0;
        bool isBinded = false;
        for ( int tries = 0; tries < 10; ++tries ) {
          if ( -1 == bind( sockfd, reinterpret_cast<struct sockaddr *>(&addr), sizeof( addr ) ) ) {
            LOG_FAILURE( "bind() for address " << (*it2)->_ip << ":" << (*it2)->_port << " failed with return code: -1, retrying..." );
          } else {
            isBinded = true;
            break ;
          }
          while ( nanosleep( &loop_delay, &loop_delay ) );
        }
        if ( false == isBinded ) {
          LOG_ERROR( "bind() failed after max retries for address " << (*it2)->_ip << ":" << (*it2)->_port << " failed with return code: -1" );
          return ( -1 );
        }

        if ( -1 == listen( sockfd, kBacklogSize ) ) {
          LOG_ERROR( "listen() for address " << (*it2)->_ip << ":" << (*it2)->_port << " failed with return code: -1" );
          return ( -1 );
        }

        this->_servers[sockfd] = new Listen( (*it2)->_ip, (*it2)->_port );
        this->addToFdSet( sockfd );
        binded.push_back( **it2 );

        LOG_INFO( "started listening on " << (*it2)->_ip << ":" << (*it2)->_port << " with fd: " << sockfd );
      }
    }
  }
  if ( true == binded.empty() ) {
    LOG_ERROR( "failed to start server" );
    return ( -1 );
  }

  LOG_SUCCESS( "server initialition successful" );

  return ( 0 );
}

} /** namespace webserv */
