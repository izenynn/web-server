#pragma once

#ifndef __CGI_HPP__
# define __CGI_HPP__

/** INCLUDES ----------------------------------- */

# include <request/RequestConfig.hpp>
# include <response/ResponseData.hpp>

/** CLASS -------------------------------------- */

namespace webserv {

class Cgi {
	public:
		Cgi( const RequestConfig & requestConfig, const ResponseData & responseData );
		~Cgi( void );

		int exec( void );

		void getHeaders( std::map<std::string, std::string> & headers );
		void getBody( std::string & body );
	private:
		int initEnv( void );

		const RequestConfig &	_requestConfig;
		const ResponseData &	_responseData;

		std::string				_requestBody;
};

} /** namespace webserv */

#endif /** __CGI_HPP__ */
