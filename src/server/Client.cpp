/** INCLUDES ----------------------------------- */

#include <server/Client.hpp>
#include <config/constants.hpp>
#include <utils/log.hpp>

/** UTILS -------------------------------------- */

/** CLASS -------------------------------------- */

namespace webserv {

Client::Client( int fd, Listen & host )
    : _fd( fd ),
      _host( host ),
      _disconnect ( false ),
      _request( webserv::nullptr ),
      _response( webserv::nullptr ),
      _requestData( webserv::nullptr ) {
  return ;
}

Client::~Client( void ) {
  if ( webserv::nullptr != this->_request ) {
    delete this->_request;
    this->_request = webserv::nullptr;
  }
  if ( webserv::nullptr != this->_response ) {
    delete this->_response;
    this->_response = webserv::nullptr;
  }
  if ( webserv::nullptr != this->_requestData ) {
    delete this->_requestData;
    this->_requestData = webserv::nullptr;
  }
  close( this->_fd );
  return ;
}

void Client::clear( void ) {
  if ( webserv::nullptr != this->_request ) {
    delete this->_request;
    this->_request = webserv::nullptr;
  }
  if ( webserv::nullptr != this->_response ) {
    delete this->_response;
    this->_response = webserv::nullptr;
  }
  if ( webserv::nullptr != this->_requestData ) {
    delete this->_requestData;
    this->_requestData = webserv::nullptr;
  }

  return ;
}

void Client::initResponse( const std::vector<ServerConfig *> & servers, int statusCode ) {
  // generate request and request config if doesnt exists
  if ( webserv::nullptr == this->_request ) {
    this->initRequest();
  }
  if ( webserv::nullptr == this->_requestData ) {
    this->initRequestData( servers );
  }

  // generate response
  this->_response = new Response( *(this->_requestData), statusCode );

  // build response
  for ( int tries = 1, redo = 1; redo != 0; ++tries ) {
    if ( tries > 10 ) {
      if ( webserv::nullptr != this->_response ) {
        delete this->_response;
      }
      this->_response = new Response( *(this->_requestData), 500 );
      this->_response->build();
      break ;
    }
    redo = 0;
    this->_response->build();
    if ( true == this->_response->getRedirect() ) {
      this->_requestData->redirect( this->_response->getRedirectUri() );
      this->_response->clear();
      redo = 1;
    }
  }

  return ;
}

void Client::initRequestData( const std::vector<ServerConfig *> & servers ) {
  this->_requestData = new RequestData( *(this->_request), this->_host, *this, servers );
  this->_requestData->initialize();
  return ;
}

void Client::initRequest( void ) {
  this->_request = new Request();
  return ;
}

int Client::getFd( void ) const {
  return ( this->_fd );
}

Request * Client::getRequest( void ) const {
  return ( this->_request );
}

Response * Client::getResponse( void ) const {
  return ( this->_response );
}

RequestData * Client::getRequestData( void ) const {
  return ( this->_requestData );
}

bool Client::getDisconnect( void ) const {
  return ( this->_disconnect );
}

void Client::setDisconnct( bool value ) {
  this->_disconnect = value;
  return ;
}

} /** namespace webserv */
