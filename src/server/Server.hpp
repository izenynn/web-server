#pragma once

#ifndef __SERVER_HPP__
# define __SERVER_HPP__

/** INCLUDES ----------------------------------- */

# include <string> // std::string
//# include <cstring> // std::memcpy()
# include <map>
# include <list>
# include <vector>
# include <algorithm> // std::find()

# include <sys/socket.h> // socket()
# include <sys/select.h> // select()
# include <fcntl.h> // fcntl()
# include <netinet/in.h> // sockaddr_in
#include <arpa/inet.h> // inet_addr()

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
		void print( void );

		int run( void );

		bool recv( int fd );
		bool send( int fd );

		void newClient( int fd );
		void disconnectClient( int fd );
		void closeClient( int fd );
		//void timeoutClient( Client * client );

		void addToFdSet( int fd );
		void delFromFdSet( int fd );
	private:
		Server & operator=( const Server & other); // not necessary

		static const char *		k_default_path;
		static const int		k_backlog_size; // listen() argument: the backlog argument defines the maximum length to which the queue of pending connections for a sockfd may grow
		static const int		k_max_clients;
		static const int		k_buffer_size;

		Config *							_config;
		const std::vector<ServerConfig *> *	_server_configs;

		std::map<int, Listen *>					_servers;
		//std::map<int, Client *>				_clients;

		std::list<int>	_fd_list;
		fd_set			_fd_set;
		fd_set			_fd_read;
		fd_set			_fd_write;
		int				_highest_fd;

		std::string		_head;
};

} /** namespace webserv */

#endif /** __SERVER_HPP__ */
