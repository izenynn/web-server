#pragma once

#ifndef __CLIENT_HPP__
# define __CLIENT_HPP__

/** INCLUDES ----------------------------------- */

# include <map> // std::map
# include <string> // std::string
# include <netinet/in.h> // sockaddr_in
# include <unistd.h> // close()
# include <fcntl.h> // fcntl()

/** CLASS -------------------------------------- */

namespace webserv {

typedef struct listen_s {
	uint32_t host;
	uint16_t port;

	listen_s() : host(0), port(0) {};
	listen_s(uint32_t host, uint16_t port) : host(host), port(port) {}; // FIXME is this legal? variables with same name as args?
} listen_t;

class Client {
	public:
		Client( const listen_t & listen );
		virtual ~Client( void );

		int		getFd( void );

		void	start( void );
		void	end( void );

		int		accept( void );
		int		send( int socket );
		int		recv( int socket );
		void	close( int socket );

		void	process( int socket );
	private:
		Client( void ); // not necessary
		Client( const Client & other ); // not necessary
		Client &operator=( const Client& other ); // not necessary

		static const size_t k_recv_size;

		int							_sockfd;
		struct sockaddr_in			_addr;
		listen_t					_host;
		std::map<int, std::string>	_requests;
};

} /** namespace webserv */

#endif /** __CONFIG_HPP__ */
