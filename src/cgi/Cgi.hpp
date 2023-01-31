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
		Cgi( void );
		~Cgi( void );

		int init( const RequestConfig & requestConfig, const ResponseData & ResponseData );
		int exec( void );

		void getHeaders( void ); // FIXME this cant be void
		void getBody( void ); // FIXME this cant be void
	private:
		int initEnv( void );

		const RequestConfig &	_requestConfig;
		const ResponseData &	_responseData;
};

} /** namespace webserv */

#endif /** __CGI_HPP__ */
