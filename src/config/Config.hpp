#pragma once

#ifndef __CONFIG_HPP__
# define __CONFIG_HPP__

/** INCLUDES ----------------------------------- */

# include <string>
# include <vector>
# include <fstream>

# include <stdint.h>

# include <types/nullptr_t.hpp>
# include <config/ServerConfig.hpp>

/** CLASS -------------------------------------- */

namespace webserv {

class Config {
	public:
		Config( const char* path ); // config class needs a config file
		virtual ~Config( void );

		//const std::vector<ServerConfig>&	getServers( void ) const;
	private:
		Config( void ); // not necessary
		Config &operator=( const Config& other ); // not necessary

		//std::vector<ServerConfig>			_servers;

		void load( const char* file );
	public:
		class ConfigException : virtual public std::exception {
			private:
				std::string message;
			public:
				ConfigException( const std::string & msg );
				~ConfigException( void ) throw ();
				const char * what() const throw ();
		};
};

} /** namespace webserv */

#endif /** __CONFIG_HPP__ */
