/** INCLUDES ----------------------------------- */

#include <server/Client.hpp>
#include <config/constants.hpp>
#include <utils/log.hpp>

/** UTILS -------------------------------------- */

/** CLASS -------------------------------------- */

namespace webserv {

Client::Client( int fd, Listen & host, bool disconnect )
		: _fd( fd ),
		  _host( host ),
		  _disconnect ( disconnect ),
		  _request( webserv::nullptr_t ),
		  _response( webserv::nullptr_t ),
		  _requestConfig( webserv::nullptr_t ) {
	return ;
}

Client::~Client( void ) {
	if ( webserv::nullptr_t != this->_request ) {
		delete this->_request;
		this->_request = webserv::nullptr_t;
	}
	if ( webserv::nullptr_t != this->_response ) {
		delete this->_response;
		this->_response = webserv::nullptr_t;
	}
	if ( webserv::nullptr_t != this->_requestConfig ) {
		delete this->_requestConfig;
		this->_requestConfig = webserv::nullptr_t;
	}
	close( this->_fd );
	return ;
}

void Client::clear( void ) {
	if ( webserv::nullptr_t != this->_request ) {
		delete this->_request;
		this->_request = webserv::nullptr_t;
	}
	if ( webserv::nullptr_t != this->_response ) {
		delete this->_response;
		this->_response = webserv::nullptr_t;
	}
	if ( webserv::nullptr_t != this->_requestConfig ) {
		delete this->_requestConfig;
		this->_requestConfig = webserv::nullptr_t;
	}
}

void Client::initResponse( const std::vector<ServerConfig *> & servers, int statusCode ) {
	log::warning("- init respose...");
	// generate request and request config if doesnt exists // FIXME necessary ???
	if ( webserv::nullptr_t == this->_request ) {
		this->initRequest();
	}
	if ( webserv::nullptr_t == this->_requestConfig ) {
		this->initRequestConfig( servers );
	}

	// generate response
	log::warning("- new response...");
	this->_response = new Response( *(this->_requestConfig), statusCode );
	log::warning("- new response ready");

	// build response
	log::warning("- building...");
	for ( int tries = 1, redo = 1; redo != 0; ++tries ) {
		log::warning("- for iteration");
		if ( tries > 10 ) {
			if ( webserv::nullptr_t != this->_response ) {
				delete this->_response;
			}
			this->_response = new Response( *(this->_requestConfig), 500 );
			this->_response->build();
			break ;
		}
		redo = 0;
		this->_response->build();
		if ( true == this->_response->getRedirect() ) {
			this->_requestConfig->redirect( this->_response->getRedirectUri() );
			this->_response->clear();
			redo = 1;
		}
	}
}

void Client::initRequestConfig( const std::vector<ServerConfig *> & servers ) {
	this->_requestConfig = new RequestConfig( *(this->_request), this->_host, *this, servers );
	this->_requestConfig->initialize();
	return ;
}

void Client::initRequest( void ) {
	this->_request = new Request();
}

bool Client::checkTimeout( void ) {
	if ( this->_request != webserv::nullptr_t ) {
		log::warning("check timeout");
		struct timeval time;
		gettimeofday( &time, NULL );
		if ( time.tv_sec - this->_request->getTime().tv_sec > kRequestTimeoutSec ) {
			return ( true );
		} else {
			return ( false );
		}
	} else {
		return ( false );
	}
}

bool Client::checkDisconnect( void ) {
	if ( true == this->_disconnect ) {
		return ( true );
	} else {
		return ( false );
	}
}

int Client::getFd( void ) {
	return ( this->_fd );
}

Request * Client::getRequest( void ) {
	return ( this->_request );
}

Response * Client::getResponse( void ) {
	return ( this->_response );
}

RequestConfig * Client::getRequestConfig( void ) {
	return ( this->_requestConfig );
}

} /** namespace webserv */
