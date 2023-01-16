#pragma once

#ifndef __SERVER_HPP__
# define __SERVER_HPP__

/** INCLUDES ----------------------------------- */

# include <string> // std::string
# include <sys/select.h> // select()
# include <cstring> // std::memcpy()
# include <map> // std::map<T, U>
//# include <sys/socket.h>
//# include <netinet/in.h>

# include <types/nullptr_t.hpp>
# include <config/Config.hpp>
# include <server/Client.hpp>

# include <stdint.h>

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

typedef struct listen_s {
	uint32_t host;
	uint32_t port;

	listen_s() : host(0), port(0) {};
	listen_s(uint32_t host, uint32_t port) : host(host), port(port) {}; // FIXME is this legal? variables with same name as args?
} listen_t;

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

		std::map<uint64_t, listen_t> _servers;
		std::map<uint64_t, Client *> _clients;

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
