/** INCLUDES ----------------------------------- */

# include <config/RequestConfig.hpp>

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

void RequestConfig::initialize( void ) {
	this->_request_uri = this->_request._request_uri;

	this->_server = this->getRequestServer();
	this->_location = this->getRequestLocation();
}

bool RequestConfig::isValidMethod( const std::string & method ) {
	const std::vector<std::string> & allowedMethods = this->_location->_limit_except;

	if ( true == allowedMethods.empty() ) {
		return ( true );
	}

	if ( allowedMethods.end() != std::find( allowedMethods.begin(), allowedMethods.end(), method ) ) {
		return ( true );
	}

	return ( false );
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

} /** namespace webserv */
