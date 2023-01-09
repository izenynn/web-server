#pragma once

#ifndef __CONFIG_HPP__
# define __CONFIG_HPP__

/** INCLUDES ----------------------------------- */

# include <string>
# include <vector>
# include <fstream>

# include <stdint.h>

/** CLASS -------------------------------------- */

namespace webserv {

typedef struct listen_s {
	uint32_t host;
	uint32_t port;

	listen_s() : host(0), port(0) {};
	listen_s(uint32_t host, uint32_t port) : host(host), port(port) {}; // FIXME is this legal? variables with same name as args?
} listen_t;

class Config {
	public:
		Config( const char* path ); // config class needs a config file
		virtual ~Config( void );

		//const std::vector<ConfigServer>&	getServers( void ) const;
	private:
		Config( void ); // not necessary
		Config &operator=( const Config& other ); // not necessary
		//std::vector<ConfigServer>			_servers;

		void load( const char* file );
	public:
		class ExtraClosingBrackets : public std::exception {
			public: const char* what() const throw();
		};
		class ExtraOpeningBrackets : public std::exception {
			public: const char* what() const throw();
		};
};

} //namespace webserv

#endif /** __CONFIG_HPP__ */
