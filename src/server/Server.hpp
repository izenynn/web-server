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
//# include <server/Client.hpp>

/** CLASS -------------------------------------- */

namespace webserv {

class Server {
	public:
		Server( void );
		~Server( void );

		void load( void ); // default path
		void load( const char* file );

		void run( void );
	private:
		Server & operator=( const Server & other); // not necessary

		static const char* k_default_path;

		Config *		_config;
		fd_set			_fd_set;
		unsigned int	_fd_cnt;

		const std::vector<ServerConfig *> *		_servers_config;

		std::map<int, Listen>					_servers;
		//std::map<int, Client *>				_clients;
	public:
		class ServerManagerException : virtual public std::exception {
			private:
				std::string message;
			public:
				ServerManagerException( const std::string & msg );
				virtual ~ServerManagerException( void ) throw ();
				virtual const char * what( void ) const throw ();
		};
};

} /** namespace webserv */

#endif /** __SERVER_HPP__ */
