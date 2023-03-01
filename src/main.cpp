#include <cstring> // std::strcmp()

#include "types.hpp"
#include "utils/log.hpp"
#include "server/Server.hpp"

int main( int argc, char * argv[] ) {
  // too many args
  if ( argc > 2 ) {
    LOG_FAILURE( "Error: too many arguments" );
    LOG_FAILURE( "Usage: " << argv[0] << " [CONF FILE]" );
    return ( 1 );
  }

  // help message
  if ( argc == 2 ) {
    if ( 0 == std::strcmp( "-h", argv[1] )
        || 0 == std::strcmp( "--help", argv[1] ) ) {
      LOG_INFO( "Usage: " << argv[0] << " [CONF FILE]" );
      return ( 0 );
    }
  }

  // web server
  webserv::unique_ptr<webserv::Server> server( new webserv::Server() );

  LOG_INFO( "loading config..." );
  try {
    if ( argc == 2 ) server->load( argv[1] );
    else             server->load();
  } catch ( std::exception & e ) {
    LOG_ERROR( e.what() );
    return ( 1 );
  }

  LOG_INFO( "starting..." );
  int ret = server->start();

  return ( ret );
}
