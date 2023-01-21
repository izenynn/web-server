/** INCLUDES ----------------------------------- */

#include <config/ServerConfig.hpp>
#include <utils/log.hpp>

namespace {
	bool isValidPort( const std::string & port ) {
		// check is all digits
		if ( std::string::npos != port.find_first_not_of( "0123456789" ) ) {
			return ( false );
		}
		// check not all 0 and skip them
		std::string::size_type start = port.find_first_not_of( "0" );
		if ( std::string::npos == start ) {
			return ( false );
		}
		// check lenght: valid range: 1 - 65535
		std::string number = port.substr( start );
		if ( number.length() < 1 || number.length() > 5 ) {
			return ( false );
		}
		if ( number.length() == 5 && number > "65535" ) {
			return ( false );
		}
		// true
		return ( true );
	}
	bool isValidIp( const std::string & ip ) {
		// check is all digits and dots
		if ( std::string::npos != ip.find_first_not_of( ".0123456789" ) ) {
			return ( false );
		}
		// check there are no more than 3 dots
		if ( 3 != std::count( ip.begin(), ip.end(), '.' ) ) {
			return ( false );
		}
		// scanf will check format and save numbers
		unsigned int n[4];
		if ( sscanf( ip.c_str(),"%d.%d.%d.%d", &(n[0]), &(n[1]), &(n[2]), &(n[3]) ) != 4 ) {
			return ( false );
		}
		// check all digits
		for ( int i = 0; i < 4; ++i ) {
			if ( n[i] < 0 || n[i] > 255 ) {
				return ( false );
			}
		}
		return ( true );
	}
}

namespace webserv {

/** CLASS -------------------------------------- */

ServerConfig::ServerConfig( void )
		: _id( -1 ) {
	this->_directivesValid["location"] =				&ServerConfig::parseLocation;
	this->_directivesValid["listen"] =					&ServerConfig::parseListen;
	this->_directivesValid["server_name"] =				&ServerConfig::parseServerName;
	this->_directivesValid["root"] =					&ServerConfig::parseRoot;
	this->_directivesValid["index"] =					&ServerConfig::parseIndex;
	this->_directivesValid["autoindex"] =				&ServerConfig::parseAutoindex;
	this->_directivesValid["error_page"] =				&ServerConfig::parseErrorPage;
	this->_directivesValid["limit_except"] =			&ServerConfig::parseLimitExcept;
	this->_directivesValid["client_max_body_size"] =	&ServerConfig::parseClientMaxBodySize;
	// TODO
	//this->_directivesValid["cgi_param"] =				&ServerConfig::parseCgiParam;
	//this->_directivesValid["cgi_pass"] =				&ServerConfig::parseCgiPass;
}

void ServerConfig::parser( token_type::const_iterator & it ) {
	// check '{' after 'server' directive
	if ( "{" != *it ) {
		log::error( "expected '{' after 'server' directive" );
		throw ServerConfig::ServerConfigException( "exception: expected '{' after 'server' directive" );
	}
	++it;

	// we check before that every '{' has a '}', so iteration must be safe without checking tokens->end()
	std::map<std::string, parse_directive_type>::const_iterator directive;
	for ( ; "}" != *it; ++it ) {
		directive = this->_directivesValid.find( *it );
		if ( this->_directivesValid.end() != directive ) {
			// check there is a next token and pass it to the directive parser function
			const token_type::const_iterator prev = it;
			++it;
			if ( "}" == *it ) {
				log::error( "missing directive '" + *prev + "' value" );
				throw ServerConfig::ServerConfigException( "exception: missing directive '" + *prev + "' value" );
			}
			( this->*( directive->second ) )( it );
		} else {
			log::error( "unknown directive: " + *it );
			throw ServerConfig::ServerConfigException( "exception: unknown directive" );
		}
	}

	return ;
}

void ServerConfig::parseLocation( token_type::const_iterator & it ) {
	(void)it;
	return ;
}

void ServerConfig::parseListen( token_type::const_iterator & it ) {
	std::string	ip = "0.0.0.0";
	uint16_t	port = 80;

	// check directive value if -> ip:port, ip, port
	std::string				token	= *it;
	std::string::size_type	sep		= token.find( ":" );
	// ip:port
	if (std::string::npos != sep ) {
		std::string tmpIp	= token.substr( 0, sep );
		std::string tmpPort	= token.substr( sep + 1 );

		if ( false == isValidIp( tmpIp ) ) {
			log::error( "invalid port on 'listen' directive" );
			throw ServerConfig::ServerConfigException( "exception: invalid port on 'listen' directive" );
		}
		if ( false == isValidPort( tmpPort ) ) {
			log::error( "invalid IP on 'listen' directive" );
			throw ServerConfig::ServerConfigException( "exception: invalid IP on 'listen' directive" );
		}

		ip		= tmpIp;
		port	= atoi( tmpPort.c_str() );
	// ip
	} else if ( true == isValidIp( *it ) ) {
		ip = *it;
	// port
	} else if ( true == isValidPort( *it ) ) {
		port = atoi( (*it).c_str() );
	// invalid
	} else {
		log::error( "invalid value on 'listen' directive" );
		throw ServerConfig::ServerConfigException( "exception: invalid value on 'listen' directive" );
	}

	// check value not duplicated
	Listen * listen = new Listen( ip, port );
	if ( this->_listens.end() != std::find( this->_listens.begin(), this-> _listens.end(), listen ) ) {
		log::error( "duplicate value in directive 'listen'" );
		throw ServerConfig::ServerConfigException( "duplicate value in directive 'listen'" );
	}

	// add to listen vector
	this->_listens.push_back( listen );

	// check nexy token is ';' (no need to check if it is '}', because if it is not ';' it will thow an erro)
	const token_type::const_iterator prev = it;
	++it;
	if ( "}" == *it ) {
		log::error( "missing ';' near token '" + *prev + "'" );
		throw ServerConfig::ServerConfigException( "exception: missing ';' near token '" + *prev + "'" );
	}
	if ( ";" != *it ) {
		log::error( "too many values in directive 'listen'" );
		throw ServerConfig::ServerConfigException( "exception: too many values in directive 'listen'" );
	}

	return ;
}

void ServerConfig::parseServerName( token_type::const_iterator & it ) {
	(void)it;
	return ;
}

ServerConfig::ServerConfigException::ServerConfigException( const std::string & msg )
		: message( msg ) {
	return ;
}

} /** namespace webserv */
