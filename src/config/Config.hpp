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
	private:
		typedef std::vector<std::string> token_type;
	public:
		Config( const char* path ); // config class needs a config file
		virtual ~Config( void );

		const std::vector<ServerConfig *> &	getServers( void ) const;
	private:
		Config( void ); // not necessary
		Config( const Config & other ); // not necessary
		Config &operator=( const Config& other ); // not necessary

		void load( const char* file );

		const token_type *	lexer( const char * const file );
		void				parser( const token_type * const tokens );

		std::vector<ServerConfig *>			_server;
	public:
		class ConfigException : virtual public std::exception {
			private:
				std::string message;
			public:
				ConfigException( const std::string & msg );
				virtual ~ConfigException( void ) throw ();
				virtual const char * what( void ) const throw ();
		};
};

} /** namespace webserv */

#endif /** __CONFIG_HPP__ */
