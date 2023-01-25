/** INCLUDES ----------------------------------- */

#include <config/RequestConfig.hpp>
#include <utils/log.hpp>

/** UTILS -------------------------------------- */

/** CLASS -------------------------------------- */

namespace webserv {

RequestConfig::RequestConfig( Request & request, Listen & host, Client & client, const std::vector<ServerConfig *> & servers ) 
		: _request( request ),
		  _host( host ),
		  _client( client ),
		  _servers( servers ) {
	return ;
}

RequestConfig::~RequestConfig( void ) {
	return ;
}

void RequestConfig::print( void ) const {
	std::string i = "    ";
	std::cout << "\nREQUEST CONFIG:" << std::endl;

	std::cout << i << "server:" << std::endl;
	this->_server->print( i + "    " );

	if ( webserv::nullptr_t != this->_location ) {
		std::cout << i << "location:" << std::endl;
		std::cout << i << "location uri: " << this->_location->first << std::endl;
		this->_location->second->print( i + "    " );
	}

	return ;
}

void RequestConfig::initialize( void ) {
	const ServerConfig *									newServer = webserv::nullptr_t;
	const std::pair<const std::string, ServerConfig *> *	newLocation = webserv::nullptr_t;

	this->_request_uri = this->_request._request_uri;

	newServer = this->getRequestServer();
	newLocation = this->getRequestLocation( newServer );

	this->_server = newServer;
	this->_location = newLocation;
}

void RequestConfig::redirect( const std::string & uri ) {
	const std::pair<const std::string, ServerConfig *> * newLocation = webserv::nullptr_t;

	newLocation = this->getRequestLocation( this->_server );

	this->_request_uri = uri;
	if ( webserv::nullptr_t != newLocation ) {
		this->_location = newLocation;
	}

	return ;
}

bool RequestConfig::isValidMethod( const std::string & method ){
	const std::vector<std::string> & allowedMethods = this->_location->second->_limit_except;

	if ( true == allowedMethods.empty() ) {
		return ( true );
	}

	if ( allowedMethods.end() != std::find( allowedMethods.begin(), allowedMethods.end(), method ) ) {
		return ( true );
	}

	return ( false );
}

std::string & RequestConfig::getMethod( void ) {
	return ( this->_request._method );
}

const std::string & RequestConfig::getBody( void ) {
	return ( this->_request._body );
}

const std::string & RequestConfig::getRequestUri( void ) {
	return ( this->_request_uri );
}

const std::string & RequestConfig::getRequestRequestUri( void ) {
	return ( this->_request._request_uri );
}

const std::string & RequestConfig::getVersion( void ) {
	return ( this->_request._version );
}

const std::map<int, std::string> &	RequestConfig::getErrorPages( void ) {
	return ( this->_location->second->_error_page );
}

std::vector<std::string> & RequestConfig::getAllowedMethods( void ) {
	return ( this->_location->second->_limit_except );
}

std::string::size_type RequestConfig::getMaxBodySize( void ) {
	return ( this->_location->second->_client_max_body_size );
}

std::vector<std::string> & RequestConfig::getIndex( void ) {
	return ( this->_location->second->_index );
}

bool RequestConfig::getAutoIndex( void ) {
	return ( this->_location->second->_autoindex );
}

const std::string & RequestConfig::getLocationUri( void ) {
	return ( this->_location->first );
}

const std::string & RequestConfig::getRoot( void ) {
	return ( this->_location->second->_root );
}

const ServerConfig * RequestConfig::getRequestServer( void ) {
	std::vector<ServerConfig *> matches;

	// match server with same ip:port
	for ( std::vector<ServerConfig *>::const_iterator it = this->_servers.begin(); it != this->_servers.end(); ++it ) {
		for ( std::vector<Listen *>::const_iterator it2 = (*it)->_listen.begin(); it2 != (*it)->_listen.end(); ++it2 ) {
			if ( ( (*it2)->ip == this->_host.ip || (*it2)->ip == "0.0.0.0" )
					&& (*it2)->port == this->_host.port ) {
				matches.push_back( *it );
				break ;
			}
		}
	}

	// if more than one match try based on server name
	if ( matches.size() > 1 ) {
		std::string host = this->_request._headers["Host"].substr( 0, this->_request._headers["Host"].find( ':' ) );
		for ( std::vector<ServerConfig *>::const_iterator it = matches.begin(); it != matches.end(); ++it ) {
			std::vector<std::string> & server_name = (*it)->getServerName();
			for ( std::vector<std::string>::const_iterator it2 = server_name.begin(); it2 != server_name.end(); ++it2 ) {
				if ( *it2 == host ) return *it;
			}
		}
	}

	// if only one match or server_name match dont found anything -> return first server
	return ( matches.front() );
}

const std::pair<const std::string, ServerConfig *> * RequestConfig::getRequestLocation( const ServerConfig * const server ) {
	const std::pair<const std::string, ServerConfig *> * match = webserv::nullptr_t;

	// find location with longest match
	for ( std::map<std::string, ServerConfig *>::const_iterator it = server->_location.begin(); it != server->_location.end(); ++it ) {
		if ( this->_request_uri.find( it->first ) ) {
			if ( webserv::nullptr_t == match ) {
				match = &(*it);
			} else if ( it->first > match->first ) {
				match = &(*it);
			}
		}
	}

	return ( match );
}

} /** namespace webserv */
