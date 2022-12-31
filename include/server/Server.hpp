#pragma once

#ifndef __SERVER_HPP__
# define __SERVER_HPP__

/** INCLUDES ----------------------------------- */

# include <string> // std::string
# include <sys/select.h> // select()
# include <cstring> // std::memcpy()
//# include <sys/socket.h>
//# include <netinet/in.h>

# include <types/nullptr_t.hpp>
# include <config/Config.hpp>

/*
===============================================================================
Server: A server listens on a connection for a request,
   parses each message received, interprets the message semantics in
   relation to the identified request target, and responds to that
   request with one or more response messages.
===============================================================================
*/

/** CLASS -------------------------------------- */

namespace webserv {

class Server {
	public:
		Server( void );
		~Server( void );

		void configLoad( void ); // default path
		void configLoad( const char* file );
		void run( void );

	private:
		Server& operator=( const Server& other); // not necessary

		static const char* DEFAULT_PATH;

		Config *		_config;
		fd_set			_fd_set;
		unsigned int	_fd_cnt;

	public:
		class ServerException : virtual public std::exception {
			private:
				std::string message;
			public:
				ServerException( const std::string& msg );
				~ServerException( void ) throw ();
				const char * what() const throw ();
		};
		class ConfigNotLoaded : public std::exception {
			public: const char * what() const throw ();
		};
};

} /** namespace webserv */

#endif /** __SERVER_HPP__ */
