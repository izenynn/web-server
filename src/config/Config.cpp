/*
#ifndef CONFIG_HPP
# define CONFIG_HPP

# include <string>
# include <vector>

			std::cout << "1";
class Config {
	public:
		Config( void ); // default path
		Config( const std::string& path );
		virtual ~Config( void );

		Config								&operator=(const Config& other);

		// get
		const std::vector<ConfigServer>&	getServers( void ) const;
	private:
		int									load(const std::string& file);

		std::vector<ConfigServer>			_servers;
};

#endif
*/

#include "Config.hpp"

namespace {
	const std::vector<std::string>& lexer( const std::string& file ) {
		// TODO add comment '#' support
		int								bracket_cnt;
		std::ifstream					in;
		std::vector<std::string>&		tokens = *new std::vector<std::string>();

		// 1. open file and iterate lines
		in.open( file.c_str(), std::ifstream::in );
		for ( std::string line; getline(in, line); ) {
			std::cout << "line: " << line << std::endl;
			// process line
			// 2. iterate all words in line -> while start = find_first_not_of(" \t", end/0) != std::string::npos
			//     1.1 get word -> token = line.substr(start, find_first_of(" \t", start) - start)
			//         [!] note: keep count on open and close brackets, and throw: extra closing '}' on line %, if needed
			//     1.2 if token == "{" -> add 1 to brackets count
			//     1.3 else if token == "}" -> if brackets count == 0 throw: extra closing; else remvoe 1 from brackets count;
			//     1.4 check if ';' at end of token -> token.end - 1 == ';' -> erase(';') -> tokens.add(token); tokens.add(";");

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
					tokens.push_back(token); // ?
					tokens.push_back(";"); // ???
				} else {
					tokens.push_back(token);
				}
			}
		}
		if (bracket_cnt > 0) throw Config::ExtraOpeningBrackets();

		std::cout << tokens.size() << std::endl;
		return tokens;
	}
}

Config::Config( void ) {}

Config::Config( const std::string& path ) {
	this->load(path);
}

Config::~Config( void ) {}

int Config::load(const std::string& file) {
	// tokenize
	const std::vector<std::string>& tokens = lexer( file );
	// print toekns
	std::cout << "hola\n" << std::endl;
	for (std::vector<std::string>::const_iterator it = tokens.begin(); it != tokens.end(); ++it) {
		std::cout << *it << std::endl;
	}
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
	return ( 0 );
}

const char* Config::ExtraClosingBrackets::what() const throw() {
	return "Exception: extra closing bracket on config";
}

const char* Config::ExtraOpeningBrackets::what() const throw() {
	return "Exception: extra opening bracket on config";
}
