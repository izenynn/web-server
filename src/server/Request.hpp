#pragma once

#ifndef __REQUEST_HPP__
# define __REQUEST_HPP__

/** INCLUDES ----------------------------------- */

# include <string>
# include <map>

/** CLASS -------------------------------------- */

namespace webserv {

class Request {
	public:
		Request( void );
		~Request( void );

		int parse( std::string & buffer );
		int parse( char * buffer, int len );

		int requestLine( void );
		int headers( void );
		int body( void );
	private:
		std::string		_method;
		std::string		_request_uri;
		std::string		_version;
		std::string		_body;
		std::map<std::string, std::string> _headers;
};

} /** namespace webserv */

#endif /** __REQUEST_HPP__ */
