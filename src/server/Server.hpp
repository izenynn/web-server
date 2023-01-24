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
# include <ctime> // nanosleep()

# include <sys/socket.h> // socket()
# include <sys/select.h> // select()
# include <fcntl.h> // fcntl()
# include <netinet/in.h> // sockaddr_in
#include <arpa/inet.h> // inet_addr()

# include <types/nullptr_t.hpp>
# include <config/Config.hpp>
# include <server/Client.hpp>

/** CLASS -------------------------------------- */

namespace webserv {

class Server {
	public:
		Server( void );
		~Server( void );

		void load( void ); // default path
		void load( const char* file );
		void print( void );

		int start( void );

		// FIXME from here to end move to private
		bool clientRecv( int fd );
		bool clientSend( int fd );

		void newClient( int fd );
		void disconnectClient( int fd );
		void checkDisconnectClient( Client * client );

		void addToFdSet( int fd );
		void delFromFdSet( int fd );
	private:
		Server & operator=( const Server & other); // not necessary

		Config *							_config;
		const std::vector<ServerConfig *> *	_serverConfigs;

		std::map<int, Listen *>				_servers;
		std::map<int, Client *>				_clients;

		std::list<int>	_fdList;
		fd_set			_fdSet;
		fd_set			_fdRead;
		fd_set			_fdWrite;
		int				_fdMax;

		int initialize( void );
};

} /** namespace webserv */

#endif /** __SERVER_HPP__ */
