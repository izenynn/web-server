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
		void redirect( const std::string & uri );

		bool isValidMethod( const std::string & method );

		// request related getters
		std::string &						getMethod( void );
		const std::string &					getBody( void );
		const std::string &					getRequestUri( void );
		const std::string &					getRequestRequestUri( void );
		const std::string &					getVersion( void );

		// server/location related getters
		const std::map<int, std::string> &	getErrorPages( void );
		std::vector<std::string> &			getAllowedMethods( void );
		std::string::size_type				getMaxBodySize( void );
		std::vector<std::string> &			getIndex( void );
		bool								getAutoIndex( void );
		const std::string &					getLocationUri( void );
		const std::string &					getRoot( void );
	private:
		RequestConfig( void ); // not necessary

		const ServerConfig *									getRequestServer( void );
		const std::pair<const std::string, ServerConfig *> *	getRequestLocation( void );

		std::string							_request_uri;

		Request &							_request;
		Listen &							_host;
		Client &							_client;

		const std::vector<ServerConfig *> &	_servers;

		const ServerConfig *									_server;
		const std::pair<const std::string, ServerConfig *> *	_location;
};

} /** namespace webserv */

#endif /** __REQUEST_CONFIG_HPP__ */
