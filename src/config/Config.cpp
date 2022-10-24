/*
#ifndef CONFIG_HPP
# define CONFIG_HPP

# include <string>
# include <vector>

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
		std::ifstream	in;

		in.open( file, std::ifstream::in );
		for ( std::string line; getline(in, line); ) {
			// process line
			// 1. remove all ' ' and '\t'
			// 2. check if it is '{', '}', or valid directive
		}
	}
}

int Config::load(const std::string& file) {
	std::vector<std::string>	tokens;

	// tokenize
	tokens = lexer( file );

	// create servers
	std::vector<std::string>::iterator it;
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
	}
	return ( 0 );
}
