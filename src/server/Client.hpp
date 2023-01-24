#pragma once

#ifndef __CLIENT_HPP__
# define __CLIENT_HPP__

/** INCLUDES ----------------------------------- */

# include <unistd.h>
# include <sys/time.h>

# include <config/Listen.hpp>
# include <config/ServerConfig.hpp>
# include <server/Request.hpp>
# include <server/Response.hpp>

/** CLASS -------------------------------------- */

namespace webserv {

class Client {
	public:
		Client( int fd, Listen & host, bool disconnect );
		~Client( void );

		void initRequest( void );
		void initResponse( const std::vector<ServerConfig *> & servers, int statusCode );
		void initRequestConfig( const std::vector<ServerConfig *> & servers );

		void clear( void );

		bool checkTimeout( void );
		bool checkDisconnect( void );

		int getFd( void );
		Request *		getRequest( void );
		Response *		getResponse( void );
		RequestConfig *	getRequestConfig( void );
	private:
		Client( void ); // not necessary
		Client( const Client & other ); // not necessary
		Client & operator=( const Client & other ); // not necessary

		int				_fd;
		Listen &		_host;
		bool			_disconnect;

		Request *		_request;
		Response *		_response;
		RequestConfig *	_requestConfig;
};

} /** namespace webserv */

#endif /** __CLIENT_HPP__ */
