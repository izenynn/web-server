#include "config/Config.hpp"

namespace {
	const std::vector<std::string>& lexer( const char* file ) {
		// TODO add comment '#' support
		std::ifstream					in;
		std::vector<std::string>&		tokens = *new std::vector<std::string>();
		int								bracket_cnt = 0;

		in.open( file, std::ifstream::in ); // TODO throw exception if cant read file
		for ( std::string line; getline(in, line); ) {
			std::string::size_type	start = line.find_first_not_of(" \t", 0);
			std::string::size_type	end = 0;
			while ( ( start = line.find_first_not_of( " \t", end ) ) != std::string::npos ) {
				end = line.find_first_of( " \t", start );
				std::string	token = line.substr( start, end - start );
				if ( token == "{" ) {
					++bracket_cnt;
				} else if ( token == "}" ) {
					if ( bracket_cnt == 0 ) throw Config::ExtraClosingBrackets();
					--bracket_cnt;
				}
				if (token.size() > 1 && *token.end() == ';') {
					token.erase(token.end() - 1);
					tokens.push_back(token);
					tokens.push_back(";");
				} else {
					tokens.push_back(token);
				}
			}
		}
		if (bracket_cnt > 0) {
			throw Config::ExtraOpeningBrackets();
		}

		return tokens;
	}
}

Config::Config( void ) {}

Config::Config( const char* path ) {
	this->load(path);
}

Config::~Config( void ) {}

void Config::load(const char* file) {
	// tokenize
	const std::vector<std::string>& tokens = lexer( file );
	// print toekns
	/*std::cout << "hola\n" << std::endl;
	for (std::vector<std::string>::const_iterator it = tokens.begin(); it != tokens.end(); ++it) {
		std::cout << *it << std::endl;
	}*/
	delete &tokens;
	// TODO check directives are valid

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

const char* Config::ExtraClosingBrackets::what() const throw() {
	return ( "Exception: extra closing bracket on config" );
}

const char* Config::ExtraOpeningBrackets::what() const throw() {
	return ( "Exception: extra opening bracket on config" );
}
