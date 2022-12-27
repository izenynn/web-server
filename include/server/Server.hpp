#pragma once

#ifndef __SERVER_HPP__
# define __SERVER_HPP__

/**	INCLUDES ----------------------------------- */

# include <string>
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

/**	METHODS   ---------------------------------- */

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

		Config *_config;

	public:
		class ConfigNotLoaded : public std::exception {
			public: const char* what() const throw();
		};
};

#endif /** __SERVER_HPP__ */
