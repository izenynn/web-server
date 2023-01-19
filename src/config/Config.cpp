/** INCLUDES ----------------------------------- */

#include <iostream>

#include <config/Config.hpp>
#include <utils/log.hpp>

namespace webserv {

/** METHODS ------------------------------------ */

namespace {

// tokenize and check for extra or missing '{}'
const std::vector<std::string> * lexer( const char* file ) {
	// TODO add comment '#' support
	std::ifstream					in;
	std::vector<std::string> *		tokens = new std::vector<std::string>();
	int								bracket_cnt = 0;

	// TODO throw exception if cant read file
	in.open( file, std::ifstream::in );

	for ( std::string line; getline(in, line); ) {
		std::string::size_type	start = line.find_first_not_of(" \t", 0);
		std::string::size_type	end = 0;
		while ( ( start = line.find_first_not_of( " \t", end ) ) != std::string::npos ) {
			end = line.find_first_of( " \t", start );
			std::string	token = line.substr( start, end - start );
			if ( token == "{" ) {
				++bracket_cnt;
			} else if ( token == "}" ) {
				if ( bracket_cnt == 0 ) {
					throw Config::ConfigException( "exception: extra one or more closing bracket on config" );
				}
				--bracket_cnt;
			}
			if (token.size() > 1 && *token.end() == ';') {
				token.erase(token.end() - 1);
				tokens->push_back(token);
				tokens->push_back(";");
			} else {
				tokens->push_back(token);
			}
		}
	}
	if (bracket_cnt > 0) {
		throw Config::ConfigException( "exception: missing one or more closing bracket on config" );
	}

	return tokens;
}
} /** namespace */

/** CLASS -------------------------------------- */

Config::Config( void ) {}

Config::Config( const char* path ) {
	this->load(path);
}

Config::~Config( void ) {}

void Config::load(const char* file) {
	// tokenize
	const std::vector<std::string> * tokens = webserv::nullptr_t;

	tokens = lexer( file );
	/*std::cout << "TOKENS:" << std::endl;
	for (std::vector<std::string>::const_iterator it = tokens->begin(); it != tokens->end(); ++it) {
		std::cout << *it << std::endl;
	}
	delete tokens;*/

	// TODO check directives are valid
	for ( std::vector<std::string>::const_iterator it = tokens->begin(); it != tokens->end(); ++it ) {
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
	}

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

Config::ConfigException::ConfigException( const std::string & msg )
		: message( msg ) {
	return ;
}

} /** namespace webserv */
