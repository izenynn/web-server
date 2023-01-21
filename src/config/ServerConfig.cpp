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
		if ( sscanf( ip.c_str(),"%u.%u.%u.%u", &(n[0]), &(n[1]), &(n[2]), &(n[3]) ) != 4 ) {
			return ( false );
		}
		// check all digits
		for ( int i = 0; i < 4; ++i ) {
			if ( n[i] > 255 ) {
				return ( false );
			}
		}
		return ( true );
	}
}

namespace webserv {

/** CLASS -------------------------------------- */

ServerConfig::ServerConfig( void )
		: _id( -1 ),
		  _autoindex( false ),
		  _client_max_body_size( 8196 ) {
	this->_serverDirectives["location"] =				&ServerConfig::parseLocation;
	this->_serverDirectives["listen"] =					&ServerConfig::parseListen;
	this->_serverDirectives["server_name"] =			&ServerConfig::parseServerName;
	this->_serverDirectives["root"] =					&ServerConfig::parseRoot;
	this->_serverDirectives["index"] =					&ServerConfig::parseIndex;
	this->_serverDirectives["autoindex"] =				&ServerConfig::parseAutoindex;
	this->_serverDirectives["error_page"] =				&ServerConfig::parseErrorPage;
	this->_serverDirectives["limit_except"] =			&ServerConfig::parseLimitExcept;
	this->_serverDirectives["client_max_body_size"] =	&ServerConfig::parseClientMaxBodySize;
	// TODO cgi on server block ??? i dont think so
	//this->_serverDirectives["cgi_param"] =				&ServerConfig::parseCgiParam;
	//this->_serverDirectives["cgi_pass"] =				&ServerConfig::parseCgiPass;

	this->_locationDirectives["root"] =					&ServerConfig::parseRoot;
	this->_locationDirectives["index"] =				&ServerConfig::parseIndex;
	this->_locationDirectives["autoindex"] =			&ServerConfig::parseAutoindex;
	this->_locationDirectives["error_page"] =			&ServerConfig::parseErrorPage;
	this->_locationDirectives["limit_except"] =			&ServerConfig::parseLimitExcept;
	this->_locationDirectives["client_max_body_size"] =	&ServerConfig::parseClientMaxBodySize;
	//this->_locationDirectives["cgi_param"] =				&ServerConfig::parseCgiParam;
	//this->_locationDirectives["cgi_pass"] =				&ServerConfig::parseCgiPass;

}

ServerConfig::~ServerConfig( void ) {
	for ( std::vector<Listen *>::iterator it = this->_listen.begin(); it != this->_listen.end(); ) {
		delete *it;
		it = this->_listen.erase( it );
	}
	for ( std::map<std::string, ServerConfig *>::iterator it = this->_location.begin(); it != this->_location.end(); ) {
		delete it->second;
		this->_location.erase( it++ );
	}
	return ;
}

void ServerConfig::print( const std::string & indent ) {
	std::cout << indent << "listen:" << std::endl;
	for ( std::vector<Listen *>::const_iterator it = this->_listen.begin(); it != this->_listen.end(); ++it ) {
		std::cout << indent << "    " << "ip: " << (*it)->ip << " port: " << (*it)->port << std::endl;
	}

	std::cout << indent << "server_name:" << std::endl;
	for ( std::vector<std::string>::const_iterator it = this->_server_name.begin(); it != this->_server_name.end(); ++it ) {
		std::cout << indent << "    " << *it << std::endl;
	}

	std::cout << indent << "root: " << this->_root << std::endl;

	std::cout << indent << "index:" << std::endl;
	for ( std::vector<std::string>::const_iterator it = this->_index.begin(); it != this->_index.end(); ++it ) {
		std::cout << indent << "    " << *it << std::endl;
	}

	std::cout << indent << "autoindex: " << std::boolalpha << this->_autoindex << std::endl;

	std::cout << indent << "error_page:" << std::endl;
	for ( std::map<int, std::string>::const_iterator it = this->_error_page.begin(); it != this->_error_page.end(); ++it ) {
		std::cout << indent << "    " << it->first << " " << it->second << std::endl;
	}

	std::cout << indent << "limit_except:" << std::endl;
	for ( std::vector<std::string>::const_iterator it = this->_limit_except.begin(); it != this->_limit_except.end(); ++it ) {
		std::cout << indent << "    " << *it << std::endl;
	}

	std::cout << indent << "client_max_body_size: " << this->_client_max_body_size << std::endl;

	if ( false == this->_location.empty() ) {
		std::cout << indent << "locations:" << std::endl;
		for ( std::map<std::string, ServerConfig *>::const_iterator it = this->_location.begin(); it != this->_location.end(); ++it ) {
			std::cout << indent << "    " << it->first << std::endl;
			it->second->print( indent + "        " );
		}
	}
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
		directive = this->_serverDirectives.find( *it );
		if ( this->_serverDirectives.end() != directive ) {
			// check there is a next token and pass it to the directive parser function
			const token_type::const_iterator prev = it;
			++it;
			if ( "}" == *it ) {
				log::error( "missing directive '" + *prev + "' value in 'server' block" );
				throw ServerConfig::ServerConfigException( "exception: missing directive '" + *prev + "' value in 'server' block" );
			}
			( this->*( directive->second ) )( it );
		} else {
			log::error( "unknown directive: '" + *it + "' in 'server' block" );
			throw ServerConfig::ServerConfigException( "exception: unknown directive: '" + *it + "' in 'server' block" );
		}
	}

	return ;
}

ServerConfig * ServerConfig::createLocationServerConfig( void ) {
	ServerConfig * location = new ServerConfig();

	location->_root					= this->_root;
	location->_index				= this->_index;
	location->_autoindex			= this->_autoindex;
	location->_error_page			= this->_error_page;
	location->_limit_except			= this->_limit_except;
	location->_client_max_body_size	= this->_client_max_body_size;
	// TODO cgi on server block ??? i dont think so
	//location->_cgi_param = this->_cgi_param;
	//location->_cgi_pass = this->_cgi_pass;

	return location;
}

void ServerConfig::parseLocation( token_type::const_iterator & it ) {
	std::string		location		= *it;
	ServerConfig *	locationConfig	= this->createLocationServerConfig();

	// check not duplicate location
	if ( this->_location.end() != this->_location.find( location ) ) {
		log::error( "duplicate value in directive 'location'" );
		throw ServerConfig::ServerConfigException( "exception: duplicate value in directive 'location'" );
	}

	// check '{' after 'location' directive
	++it;
	if ( "{" != *it ) {
		log::error( "expected '{' after 'location' directive" );
		throw ServerConfig::ServerConfigException( "exception: expected '{' after 'location' directive" );
		
	}
	++it;

	// parse location block
	std::map<std::string, parse_directive_type>::const_iterator directive;
	for ( ; "}" != *it; ++it ) {
		directive = this->_locationDirectives.find( *it );
		if ( this->_locationDirectives.end() != directive ) {
			// check there is a next token and pass it to the directive parser function
			const token_type::const_iterator prev = it;
			++it;
			if ( "}" == *it ) {
				log::error( "missing directive '" + *prev + "' value in 'location' block" );
				throw ServerConfig::ServerConfigException( "exception: missing directive '" + *prev + "' value in 'location' block" );
			}
			//( this->*( directive->second ) )( it );
			( locationConfig->*( directive->second ) )( it );
		} else {
			log::error( "unknown directive: '" + *it + "' in 'location' block" );
			throw ServerConfig::ServerConfigException( "exception: unknown directive: '" + *it + "' in 'location' block" );
		}

	}

	// add location to map
	this->_location[location] = locationConfig;

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
	if ( this->_listen.end() != std::find( this->_listen.begin(), this-> _listen.end(), listen ) ) {
		log::error( "duplicate value in directive 'listen'" );
		throw ServerConfig::ServerConfigException( "duplicate value in directive 'listen'" );
	}

	// add to listen vector
	this->_listen.push_back( listen );

	// check nexy token is ';' (no need to check if it is '}', because if it is not ';' it will thow an erro)
	++it;
	if ( "}" == *it ) {
		log::error( "missing ';' near token 'listen'" );
		throw ServerConfig::ServerConfigException( "exception: missing ';' near token 'listen'" );
	}
	if ( ";" != *it ) {
		log::error( "too many values in directive 'listen'" );
		throw ServerConfig::ServerConfigException( "exception: too many values in directive 'listen'" );
	}

	return ;
}

void ServerConfig::parseServerName( token_type::const_iterator & it ) {
	// check next token is not an end token, server_name requires a value
	if ( "}" == *it ) {
		log::error( "missing value and ';' near token 'server_name'" );
		throw ServerConfig::ServerConfigException( "exception: missing value and ';' near token 'server_name'" );
	}
	if ( ";" == *it ) {
		log::error( "not enough values in directive 'server_name'" );
		throw ServerConfig::ServerConfigException( "exception: not enough values in directive 'server_name'" );
	}

	// iterate server names
	for ( ; ";" != *it; ++it ) {
		// if token is '}' that means the ';' is missing
		if ( "}" == *it ) {
			log::error( "missing ';' near token 'server_name'" );
			throw ServerConfig::ServerConfigException( "exception: missing ';' near token 'server_name'" );
		}
		// save directive
		this->_server_name.push_back( *it );
	}

	return ;
}

void ServerConfig::parseRoot( token_type::const_iterator & it ) {
	// check next token is not an end token, root requires a value
	if ( "}" == *it ) {
		log::error( "missing value and ';' near token 'root'" );
		throw ServerConfig::ServerConfigException( "exception: missing value and ';' near token 'root'" );
	}
	if ( ";" == *it ) {
		log::error( "not enough values in directive 'root'" );
		throw ServerConfig::ServerConfigException( "exception: not enough values in directive 'root'" );
	}

	// save directive
	this->_root = *it;

	// check next token is ';'
	++it;
	if ( "}" == *it ) {
		log::error( "missing ';' near token 'root'" );
		throw ServerConfig::ServerConfigException( "exception: missing ';' near token 'root'" );
	}
	if ( ";" != *it ) {
		log::error( "too many values in directive 'root'" );
		throw ServerConfig::ServerConfigException( "exception: too many values in directive 'root'" );
	}

	return ;
}

void ServerConfig::parseIndex( token_type::const_iterator & it ) {
	// check next token is not an end token, server_name requires a value
	if ( "}" == *it ) {
		log::error( "missing value and ';' near token 'index'" );
		throw ServerConfig::ServerConfigException( "exception: missing value and ';' near token 'index'" );
	}
	if ( ";" == *it ) {
		log::error( "not enough values in directive 'index'" );
		throw ServerConfig::ServerConfigException( "exception: not enough values in directive 'index'" );
	}

	// iterate server names
	for ( ; ";" != *it; ++it ) {
		// if token is '}' that means the ';' is missing
		if ( "}" == *it ) {
			log::error( "missing ';' near token 'index'" );
			throw ServerConfig::ServerConfigException( "exception: missing ';' near token 'index'" );
		}
		// save directive
		this->_index.push_back( *it );
	}

	return ;
}

void ServerConfig::parseAutoindex( token_type::const_iterator & it ) {
	// check next token is not an end token, server_name requires a value
	if ( "}" == *it ) {
		log::error( "missing value and ';' near token 'autoindex'" );
		throw ServerConfig::ServerConfigException( "exception: missing value and ';' near token 'autoindex'" );
	}
	if ( ";" == *it ) {
		log::error( "not enough values in directive 'autoindex'" );
		throw ServerConfig::ServerConfigException( "exception: not enough values in directive 'autoindex'" );
	};

	// save directive
	if ( "on" == *it ) {
		this->_autoindex = true;
	} else if ( "off" == *it ) {
		this->_autoindex = false;
	} else {
		log::error( "unknown value: '" + *it + "' in 'autoindex' directive" );
		throw ServerConfig::ServerConfigException( "exception: unknown value: '" + *it + "' in 'autoindex' directive" );
	}

	// check next token is ';'
	++it;
	if ( "}" == *it ) {
		log::error( "missing ';' near token 'autoindex'" );
		throw ServerConfig::ServerConfigException( "exception: missing ';' near token 'autoindex" );
	}
	if ( ";" != *it ) {
		log::error( "too many values in directive 'autoindex'" );
		throw ServerConfig::ServerConfigException( "exception: too many values in directive 'autoindex'" );
	}

	return ;
}

void ServerConfig::parseErrorPage( token_type::const_iterator & it ) {
	// check next token is not an end token, server_name requires a value
	if ( "}" == *it ) {
		log::error( "missing value and ';' near token 'error_page'" );
		throw ServerConfig::ServerConfigException( "exception: missing value and ';' near token 'error_page'" );
	}
	if ( ";" == *it ) {
		log::error( "not enough values in directive 'error_page'" );
		throw ServerConfig::ServerConfigException( "exception: not enough values in directive 'error_page'" );
	};

	// save directive
	std::vector<int> codes;
	while ( std::string::npos == it->find_first_not_of( "0123456789" ) ) {
		codes.push_back( atoi( it->c_str() ) );
		++it;
	}
	if ( "}" == *it ) {
		log::error( "missing value and ';' near token 'error_page'" );
		throw ServerConfig::ServerConfigException( "exception: missing value and ';' near token 'error_page'" );
	}
	if ( ";" == *it ) {
		log::error( "not enough values in directive 'error_page'" );
		throw ServerConfig::ServerConfigException( "exception: not enough values in directive 'error_page'" );
	};
	for ( std::vector<int>::const_iterator n = codes.begin(); n != codes.end(); ++n ) {
		this->_error_page[*n] = *it;
	}

	// check next token is ';'
	++it;
	if ( "}" == *it ) {
		log::error( "missing ';' near token 'error_page'" );
		throw ServerConfig::ServerConfigException( "exception: missing ';' near token 'error_page'" );
	}
	if ( ";" != *it ) {
		log::error( "too many values in directive 'error_page'" );
		throw ServerConfig::ServerConfigException( "exception: too many values in directive 'error_page'" );
	}

	return ;
}

void ServerConfig::parseLimitExcept( token_type::const_iterator & it ) {
	// check next token is not an end token
	if ( "}" == *it ) {
		log::error( "missing value and ';' near token 'limit_except'" );
		throw ServerConfig::ServerConfigException( "exception: missing value and ';' near token 'limit_except'" );
	}
	if ( ";" == *it ) {
		log::error( "not enough values in directive 'limit_except'" );
		throw ServerConfig::ServerConfigException( "exception: not enough values in directive 'limit_except'" );
	}

	// iterate allowed methods
	for ( ; ";" != *it; ++it ) {
		// if token is '}' that means the ';' is missing
		if ( "}" == *it ) {
			log::error( "missing ';' near token 'limit_except'" );
			throw ServerConfig::ServerConfigException( "exception: missing ';' near token 'limit_except'" );
		}
		// save directive
		this->_limit_except.push_back( *it );
	}

	return ;
}

void ServerConfig::parseClientMaxBodySize( token_type::const_iterator & it ) {
	// check next token is not an end token
	if ( "}" == *it ) {
		log::error( "missing value and ';' near token 'client_max_body_size'" );
		throw ServerConfig::ServerConfigException( "exception: missing value and ';' near token 'client_max_body_size'" );
	}
	if ( ";" == *it ) {
		log::error( "not enough values in directive 'client_max_body_size'" );
		throw ServerConfig::ServerConfigException( "exception: not enough values in directive 'client_max_body_size'" );
	};

	// save directive
	if ( std::string::npos != it->find_first_not_of( "0123456789" ) ) {
		log::error( "invalid size in 'client_max_body_size' directive" );
		throw ServerConfig::ServerConfigException( "exception: invalid size in 'client_max_body_size' directive" );
	}
	this->_client_max_body_size = atoi( it->c_str() );

	// check next token is ';'
	++it;
	if ( "}" == *it ) {
		log::error( "missing ';' near token 'client_max_body_size'" );
		throw ServerConfig::ServerConfigException( "exception: missing ';' near token 'client_max_body_size'" );
	}
	if ( ";" != *it ) {
		log::error( "too many values in directive 'client_max_body_size'" );
		throw ServerConfig::ServerConfigException( "exception: too many values in directive 'client_max_body_size'" );
	}

	return ;
}

ServerConfig::ServerConfigException::ServerConfigException( const std::string & msg )
		: message( msg ) {
	return ;
}
ServerConfig::ServerConfigException::~ServerConfigException( void ) throw () {
	return ;
}
const char * ServerConfig::ServerConfigException::what( void ) const throw () {
	return this->message.c_str();
}

} /** namespace webserv */
