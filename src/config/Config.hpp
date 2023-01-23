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

		static const char *						kDefaultPath;
		static const int						kBacklogSize; // listen() argument: the backlog argument defines the maximum length to which the queue of pending connections for a sockfd may grow
		static const int						kMaxClients;
		static const int						kBufferSize;
		static const time_t						kTimeoutSec;
		static const long						kNsecLoopDelay;
		static const std::string::size_type		kLimitRequestLimit;

		void load( const char* file );

		void print( void );

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
