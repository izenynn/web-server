/** INCLUDES ----------------------------------- */

# include "utils/signals.h"

# include "utils/log.h"

/** METHODS ------------------------------------ */

namespace {

void sig_sigint( int signal ) {
  if ( SIGINT == signal ) {
    LOG_INFO( "SIGINT detected, stoping..." );
    webserv::Server::stop();
  }
  return ;
}

void sig_sigquit( int signal ) {
  if ( SIGQUIT == signal ) {
    LOG_INFO( "SIGQUIT detected, stoping..." );
    webserv::Server::stop();
  }
  return ;
}

} /** namespace */

namespace webserv {

namespace signals {

void handle_signals( void ) {
  std::signal( SIGINT, ::sig_sigint );
  std::signal( SIGQUIT, ::sig_sigquit );
  return ;
}

} // namespace signals

} // namespace webserv
