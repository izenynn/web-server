#pragma once

#ifndef __REQUEST_CONFIG_HPP__
# define __REQUEST_CONFIG_HPP__

/** INCLUDES ----------------------------------- */

# include <request/Request.hpp>

/** CLASS -------------------------------------- */

namespace webserv {

class Client;

class RequestConfig {
	public:
		RequestConfig( Request & request, Listen & host, Client & client, const std::vector<ServerConfig *> & servers );
		~RequestConfig( void );

		void print( void ) const;

		void initialize( void );
		void redirect( const std::string & uri );

		bool isValidMethod( const std::string & method );

		// request related getters
		std::string &						getMethod( void ) const;
		const std::string &					getBody( void ) const;
		const std::string &					getRequestUri( void ) const;
		const std::string &					getRequestRequestUri( void ) const;
		const std::string &					getVersion( void ) const;
		const std::string &					getHost( void ) const;
		uint16_t							getPort( void ) const;
		const std::map<std::string, std::string> & getHeaders( void ) const;

		// server/location related getters
		const std::map<int, std::string> &	getErrorPages( void ) const;
		std::vector<std::string> &			getAllowedMethods( void ) const;
		std::string::size_type				getMaxBodySize( void ) const;
		std::vector<std::string> &			getIndex( void ) const;
		bool								getAutoIndex( void ) const;
		const std::string &					getLocationUri( void ) const;
		const std::string &					getRoot( void ) const;
		const std::string &					getUploadStore( void ) const;
		const std::string &					getAlias( void ) const;
		const std::pair<int, std::string> &	getReturn( void ) const;
		const std::map<std::string, std::string> & getCgi( void ) const;
	private:
		RequestConfig( void ); // not necessary

		ServerConfig *											getRequestServer( void );
		const std::pair<const std::string, ServerConfig *> *	getRequestLocation( const ServerConfig * const server );

		std::string							_request_uri;

		Request &							_request;
		Listen &							_host;
		Client &							_client;

		const std::vector<ServerConfig *> &	_servers;

		const ServerConfig *									_server;
		const std::pair<const std::string, ServerConfig *> *	_location;
		bool													_locationAllocated;
};

} /** namespace webserv */

#endif /** __REQUEST_CONFIG_HPP__ */
