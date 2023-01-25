#pragma once

#ifndef __RESPONSE_HPP__
# define __RESPONSE_HPP__

/** INCLUDES ----------------------------------- */

# include <map>
# include <string>
# include <iostream>

# include <config/RequestConfig.hpp>
# include <server/Uri.hpp>

/** CLASS -------------------------------------- */

namespace webserv {

class Response {
	public:
		Response( RequestConfig & config, int statusCode );
		~Response( void );

		void build( void );

		const std::string & getResponseBody( void );
		bool getRedirect( void );
		std::string getRedirectUri( void );

		bool isConnectionClose( void );
	private:
		Response( void ); // not necessary
		Response( const Response & other ); // not necessary
		Response & operator=( const Response & other ); // not necessary

		static std::map<int, std::string>			kStatusCodes;
		static std::map<std::string, std::string>	kMimeTypes;

		static std::map<int, std::string>			initStatusCodes( void );
		static std::map<std::string, std::string>	initMimeTypes( void );

		typedef int ( Response::*method )( void );

		int		process( void );
		void	setResponse( void );

		int methodGet( void );
		int methodPost( void );
		int methodPut( void );
		int methodDelete( void );
		std::map<std::string, Response::method> _methods;

		void generateErrorPage( int statusCode );

		bool			_redirect;
		std::string		_redirect_uri;
		int				_redirect_status_code;

		std::string							_response;

		int									_statusCode;
		RequestConfig &						_requestConfig;
		std::map<std::string, std::string>	_headers;
		std::string							_body;

		Uri									_uri;
};

} /** namespace webserv */

#endif /** __RESPONSE_HPP__ */
