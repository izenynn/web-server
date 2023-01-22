#pragma once

#ifndef __CLIENT_HPP__
# define __CLIENT_HPP__

/** INCLUDES ----------------------------------- */

# include <config/Listen.hpp>
# include <config/ServerConfig.hpp>

/** CLASS -------------------------------------- */

namespace webserv {

class Client {
	public:
		Client( int fd, Listen * host, bool disconnect );
		~Client( void );

		void setConfig( std::vector<ServerConfig *> & servers );
		void setResponse( std::vector<ServerConfig *> & servers, int error_code );

		void clear( void );

		bool checkTimeout( void );
		bool checkDisconnect( void );

		int getFd( void );
		Request *		getRequest( bool force );
		Response *		getResponse( void );
		RequestConfig *	getRequestConfig();
	private:
		Client( void ); // not necessary
		Client( const Client & other ); // not necessary
		Client & operator=( const Client & other ); // not necessary

		int				_fd;
		Listen &		_host;
		bool			_disconnect;
		Request *		_request;
		Response *		_response;
		RequestConfig *	_config;
};

} /** namespace webserv */

#endif /** __CLIENT_HPP__ */
