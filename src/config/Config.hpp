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
		Config( void );
		virtual ~Config( void );

		void load( const char* file );

		const std::vector<ServerConfig *> *	getServers( void ) const;
	private:
		Config( const Config & other ); // not necessary
		Config &operator=( const Config& other ); // not necessary

		const char *						_file;
		std::vector<std::string>			_tokens;
		std::vector<ServerConfig *>			_server;

		void				lexer( void );
		void				parser( void );
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
