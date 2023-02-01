/** INCLUDES ----------------------------------- */

#include <request/RequestConfig.hpp>
#include <config/constants.hpp>
#include <utils/log.hpp>

/** UTILS -------------------------------------- */

/** CLASS -------------------------------------- */

namespace webserv {

RequestConfig::RequestConfig( Request & request, Listen & host, Client & client, const std::vector<ServerConfig *> & servers ) 
		: _request( request ),
		  _host( host ),
		  _client( client ),
		  _servers( servers ),
		  _locationAllocated( false ) {
	return ;
}

RequestConfig::~RequestConfig( void ) {
	if ( true == this->_locationAllocated ) {
		delete this->_location;
	}
	return ;
}

void RequestConfig::print( void ) const {
	std::string i = "    ";
	std::cout << "\nREQUEST CONFIG:" << std::endl;

	/*std::cout << i << "server:" << std::endl;
	this->_server->print( i + "    " );

	if ( webserv::nullptr_t != this->_location ) {
		std::cout << i << "location:" << std::endl;
		std::cout << i << "location uri: " << this->_location->first << std::endl;
		this->_location->second->print( i + "    " );
	}*/

	return ;
}

void RequestConfig::initialize( void ) {
	ServerConfig *											newServer = webserv::nullptr_t;
	const std::pair<const std::string, ServerConfig *> *	newLocation = webserv::nullptr_t;

	this->_request_uri = this->_request._requestUri;

	newServer = this->getRequestServer();
	newLocation = this->getRequestLocation( newServer );

	// assign server
	this->_server = newServer;
	// delete previus location
	if ( webserv::nullptr_t != this->_location && true == this->_locationAllocated ) {
		delete this->_location;
		this->_locationAllocated = false;
	}
	// assign location
	if ( webserv::nullptr_t != newLocation ) {
		this->_location = newLocation;
	} else {
		this->_location = new std::pair<const std::string, ServerConfig *>( std::string( newServer->_root ), newServer );
		this->_locationAllocated = true;
	}
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

std::string & RequestConfig::getMethod( void ) const {
	return ( this->_request._method );
}

const std::string & RequestConfig::getBody( void ) const {
	return ( this->_request._body );
}

const std::string & RequestConfig::getRequestUri( void ) const {
	return ( this->_request_uri );
}

const std::string & RequestConfig::getRequestQuery( void ) const {
	return ( this->_request._requestQuery );
}

const std::string & RequestConfig::getRequestRequestUri( void ) const {
	return ( this->_request._requestUri );
}

const std::string & RequestConfig::getVersion( void ) const {
	if ( true == this->_request._version.empty() ) {
		return ( kDefaultVersion );
	}
	return ( this->_request._version );
}

const std::string & RequestConfig::getHost( void ) const {
	return ( this->_host.ip );
}

uint16_t RequestConfig::getPort( void ) const {
	return ( this->_host.port );
}

const std::map<int, std::string> &	RequestConfig::getErrorPages( void ) const {
	return ( this->_location->second->_error_page );
}

std::vector<std::string> & RequestConfig::getAllowedMethods( void ) const {
	return ( this->_location->second->_limit_except );
}

std::string::size_type RequestConfig::getMaxBodySize( void ) const {
	return ( this->_location->second->_client_max_body_size );
}

std::vector<std::string> & RequestConfig::getIndex( void ) const {
	return ( this->_location->second->_index );
}

bool RequestConfig::getAutoIndex( void ) const {
	return ( this->_location->second->_autoindex );
}

const std::string & RequestConfig::getLocationUri( void ) const {
	return ( this->_location->first );
}

const std::string & RequestConfig::getRoot( void ) const {
	return ( this->_location->second->_root );
}

const std::string & RequestConfig::getUploadStore( void ) const {
	return ( this->_location->second->_upload_store );
}

const std::string & RequestConfig::getAlias( void ) const {
	return ( this->_location->second->_alias );
}

const std::pair<int, std::string> &	RequestConfig::getReturn( void ) const {
	return ( this->_location->second->_return );
}

const std::map<std::string, std::string> & RequestConfig::getCgi( void ) const {
	return ( this->_location->second->_cgi );
}

const std::map<std::string, std::string> & RequestConfig::getHeaders( void ) const {
	return ( this->_request._headers );
}

ServerConfig * RequestConfig::getRequestServer( void ) {
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
		log::debug( "trying location... " + it->first );
		if ( 0 == this->_request_uri.find( it->first ) ) {
			log::debug( "match... " + it->first );
			if ( webserv::nullptr_t == match ) {
				log::debug( "first match! " + it->first + " and " + this->_request_uri );
				match = &(*it);
			} else if ( it->first.length() > match->first.length() ) {
				log::debug( "match! " + it->first + " and " + this->_request_uri );
				match = &(*it);
			}
		}
	}

	return ( match );
}

} /** namespace webserv */
