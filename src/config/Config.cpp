/** INCLUDES ----------------------------------- */

#include <config/Config.hpp>
#include <utils/log.hpp>
#include <config/constants.hpp>

/** CLASS -------------------------------------- */

namespace webserv {

Config::Config( void ) {
	return ;
}

Config::~Config( void ) {
	for ( std::vector<ServerConfig *>::iterator it = this->_server.begin(); it != this->_server.end(); ) {
		delete *it;
		it = this->_server.erase( it );
	}
	return ;
}

void Config::print( void ) {
	log::info( "TOKENS: " );
	for ( std::vector<std::string>::const_iterator it = this->_tokens.begin(); it != this->_tokens.end(); ++it ) {
		std::cout << *it << std::endl;
	}

	log::info( "CONFIG CLASS INFO AFTER PARSER: " );
	for ( std::vector<ServerConfig *>::const_iterator it = this->_server.begin(); it != this->_server.end(); ++it ) {
		(*it)->print( "" );
	}
}

void Config::load(const char * const file) {
	this->_file = file;

	this->lexer();
	this->parser();

	return ;
}

const std::vector<ServerConfig *> * Config::getServers( void ) const {
	return ( &( this->_server ) );
}

// tokenize and check for extra or missing '{}'
void Config::lexer( void ) {
	// TODO add comment '#' support
	std::ifstream					in;

	// open file
	in.open( this->_file, std::ifstream::in );
	if ( false == in ) {
		throw Config::ConfigException( "exception: can't open config file: " + std::string( this->_file ) );
	}

	// tokenize line by line
	for ( std::string line; getline(in, line); ) {
		std::string::size_type	start = line.find_first_not_of(" \t", 0);
		std::string::size_type	end = 0;
		while ( ( start = line.find_first_not_of( " \t", end ) ) != std::string::npos ) {
			end = line.find_first_of( " \t", start );
			std::string	token = line.substr( start, end - start );

			// save token, if semicolon separate it from directive
			if ( token.length() > 1 && token[token.length() - 1] == ';' ) {
				token.erase(token.end() - 1);
				this->_tokens.push_back(token);
				this->_tokens.push_back(";");
			} else {
				this->_tokens.push_back(token);
			}
		}
	}

	// close file
	in.close();
}

// parse tokens into actual config and run basic checks
void Config::parser( void ) {
	// check for brackets
	int bracketCnt = 0;
	for ( token_type::const_iterator it = this->_tokens.begin(); it != this->_tokens.end(); ++it ) {
		if ( "{" == *it ) {
			++bracketCnt;
		} else if ( "}" == *it ) {
			if ( 0 == bracketCnt ) {
				throw Config::ConfigException( "exception: extra one or more closing bracket on config" );
			}
			--bracketCnt;
		}
	}
	if ( bracketCnt > 0 ) {
		throw Config::ConfigException( "exception: missing one or more closing bracket on config" );
	}

	// parse tokens
	int serverCnt = 0;
	for ( token_type::const_iterator it = this->_tokens.begin(); it != this->_tokens.end(); ++it ) {
		if ( "server" == *it ) {
			ServerConfig * server = new ServerConfig();

			//server->setId = serverCnt;
			try {
				server->parser( ++it );
			} catch ( std::exception & e ) {
				delete server;
				log::error( e.what() );
				throw Config::ConfigException( "exception: server block parser throw an exception" );
			}

			this->_server.push_back( server );

			++serverCnt;
		} else {
			throw Config::ConfigException( "exception: unknown directive" );
		}
	}
	if ( serverCnt == 0 ) {
		throw Config::ConfigException( "exception: missing 'server' directive" );
	}

	return ;
}

Config::ConfigException::ConfigException( const std::string & msg )
		: message( msg ) {
	return ;
}
Config::ConfigException::~ConfigException( void ) throw () {
	return ;
}
const char * Config::ConfigException::what( void ) const throw () {
	return this->message.c_str();
}

} /** namespace webserv */
