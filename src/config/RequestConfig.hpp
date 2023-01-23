#pragma once

#ifndef __REQUEST_CONFIG_HPP__
# define __REQUEST_CONFIG_HPP__

/** INCLUDES ----------------------------------- */

# include <server/Request.hpp>

/** CLASS -------------------------------------- */

namespace webserv {

class Client;

class RequestConfig {
	public:
		RequestConfig( Request & request, Listen & host, Client & client, const std::vector<ServerConfig *> & servers );
		~RequestConfig( void );

		void initialize( void );

		bool isValidMethod( const std::string & method );
	private:
		RequestConfig( void ); // not necessary

		ServerConfig *	getRequestServer( void );
		ServerConfig *	getRequestLocation( void );

		std::string							_request_uri;

		Request &							_request;
		Listen &							_host;
		Client &							_client;

		const std::vector<ServerConfig *> &	_servers;

		const ServerConfig *				_server;
		const ServerConfig *				_location;
};

} /** namespace webserv */

#endif /** __REQUEST_CONFIG_HPP__ */
