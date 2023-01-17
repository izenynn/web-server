#pragma once

#ifndef __CLIENT_HPP__
# define __CLIENT_HPP__

/** INCLUDES ----------------------------------- */

# include <map> // std::map
# include <string> // std::string
# include <netinet/in.h> // sockaddr_in

/** CLASS -------------------------------------- */

namespace webserv {

typedef struct listen_s {
	uint32_t host;
	uint32_t port;

	listen_s() : host(0), port(0) {};
	listen_s(uint32_t host, uint32_t port) : host(host), port(port) {}; // FIXME is this legal? variables with same name as args?
} listen_t;

class Client {
	public:
		//Client( void ); // TODO custom constructor
		virtual ~Client( void );

		int getFd( void );

		void	configLoad( void );
	private:
		Client( void ); // not necessary
		Client &operator=( const Client& other ); // not necessary

		int							_fd;
		struct sockaddr_in			_addr;
		//std::string					_addr; // ???
		listen_t					_listen;
		//listen_t					_host; // ???
		std::map<long, std::string>	_requests;
};

} /** namespace webserv */

#endif /** __CONFIG_HPP__ */
