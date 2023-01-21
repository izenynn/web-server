/** INCLUDES ----------------------------------- */

#include <config/Config.hpp>
#include <utils/log.hpp>

namespace webserv {

/** CLASS -------------------------------------- */

Config::Config( void ) {}

Config::Config( const char* path ) {
	this->load(path);
}

Config::~Config( void ) {}

const std::vector<ServerConfig *> & Config::getServers( void ) const {
	return ( this->_servers );
}

void Config::load(const char * const file) {
	// tokenize
	const std::vector<std::string> * tokens = webserv::nullptr_t;

	tokens = this->lexer( file );
	this->parser( tokens );

	delete tokens;

	/*std::cout << "TOKENS:" << std::endl;
	for (std::vector<std::string>::const_iterator it = tokens->begin(); it != tokens->end(); ++it) {
		std::cout << *it << std::endl;
	}
	delete tokens;*/

	// deprecated
	/*for ( std::vector<std::string>::const_iterator it = tokens->begin(); it != tokens->end(); ++it ) {
		if ( "server" == *it ) {
			// TODO create a class for the server config and move all this to the server config class
			ServerConfig srvConf;
			if ( ++it, "{" == *it ) {
				log::error( "expected '{' after 'server' directive" );
				delete tokens;
				throw Config::ConfigException( "exception: expected '{' after 'server' directive" );
			}
			// TODO create parse method and send all tokens between the '{}'
			// TODO parse must retucn the it pointing to the last element, '}' i think
			if ( ++it, -1 == srvConf.parse( tokens, it ) ) {
				log::error( "error parsing 'server' directive on token: " + SSTR( *it ) );
				delete tokens;
				throw Config::ConfigException( "exception: error while parsing 'server' directive" );
			}
		} else {
			log::error( "unknown directive: " + SSTR( *it ) );
			delete tokens;
			throw Config::ConfigException( "exception: unknown directive" );
		}
	}*/

	// create servers
	/*std::vector<std::string>::iterator it;
	if (this->_servers.empty()) {
		// no server block error
		return ( 1 );
	}
	for (it = tokens.begin(); it != tokens.end(); ++it) {
		if (*it == "server") {
			// save server to vector
		} else {
			// invalid directive
		}
	}*/
	return ;
}

// tokenize and check for extra or missing '{}'
const Config::token_type * Config::lexer( const char * const file ) {
	// TODO add comment '#' support
	std::ifstream					in;
	std::vector<std::string> *		tokens = new std::vector<std::string>();

	// open file
	in.open( file, std::ifstream::in );
	if ( false == in ) {
		delete tokens;
		log::error( "can't open config file: " + std::string( file ) );
		throw Config::ConfigException( "exception: can't open config file: " + std::string( file ) );
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
				tokens->push_back(token);
				tokens->push_back(";");
			} else {
				tokens->push_back(token);
			}
		}
	}

	// close file
	in.close();

	return tokens;
}

// parse tokens into actual config and run basic checks
void Config::parser( const Config::token_type * const tokens ) {
	// check for brackets
	int bracketCnt = 0;
	for ( token_type::const_iterator it = tokens->begin(); it != tokens->end(); ++it ) {
		if ( "{" == *it ) {
			++bracketCnt;
		} else if ( "}" == *it ) {
			if ( 0 == bracketCnt ) {
				log::error( "extra one or more closing bracket on config" );
				throw Config::ConfigException( "exception: extra one or more closing bracket on config" );
			}
			--bracketCnt;
		}
	}
	if ( bracketCnt > 0 ) {
		log::error( "missing one or more closing bracket on config " );
		throw Config::ConfigException( "exception: missing one or more closing bracket on config" );
	}

	// parse tokens
	int serverCnt = 0;
	for ( token_type::const_iterator it = tokens->begin(); it != tokens->end(); ++it ) {
		if ( "server" == *it ) {
			ServerConfig * server = new ServerConfig();

			//server->setId = serverCnt;
			server->parser( ++it );

			this->_servers.push_back( server );

			++serverCnt;
		} else {
			log::error( "unknown directive: " + *it );
			throw Config::ConfigException( "exception: unknown directive" );
		}
	}
	if ( serverCnt == 0 ) {
		log::error( "missing 'server' directive" );
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
