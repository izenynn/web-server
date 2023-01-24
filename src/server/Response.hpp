#pragma once

#ifndef __RESPONSE_HPP__
# define __RESPONSE_HPP__

/** INCLUDES ----------------------------------- */

# include <map>
# include <string>

# include <config/RequestConfig.hpp>

/** CLASS -------------------------------------- */

namespace webserv {

class Response {
	public:
		Response( RequestConfig & config, int statusCode );
		~Response( void );

		bool getRedirect( void );
		std::string getRedirectUri( void );
	private:
		Response( void ); // not necessary
		Response( const Response & other ); // not necessary
		Response & operator=( const Response & other ); // not necessary

		static std::map<int, std::string>			kStatusCodes;
		static std::map<std::string, std::string>	kMimeTypes;

		static std::map<int, std::string>			initStatusCodes( void );
		static std::map<std::string, std::string>	initMimeTypes( void );

		typedef int ( Response::*method )();

		int GET( void );
		int POST( void );
		int PUT( void );
		int DELETE( void );
		std::map<std::string, Response::method> _methods;

		int		_statusCode;

		void generateErrorPage( int statusCode );

		bool			_redirect;
		std::string		_redirect_uri;
		int				_redirect_status_code;

		RequestConfig &						_requestConfig;
		std::map<std::string, std::string>	_headers;
		std::string							_body;
};

} /** namespace webserv */

#endif /** __RESPONSE_HPP__ */
