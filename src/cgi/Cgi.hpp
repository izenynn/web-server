#pragma once

#ifndef __CGI_HPP__
# define __CGI_HPP__

/** INCLUDES ----------------------------------- */

# include <string>
# include <algorithm>
# include <cstring> // std::strdup()

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
		int setEnv( void );

		const RequestConfig &	_requestConfig;
		const ResponseData &	_responseData;

		std::string				_reqBody;
		std::string				_reqFilePath;

		std::string				_cgiBin;
		std::string				_cgiPath;

		std::string				_cgiTmpFilePath;
		int						_cgiTmpFileFd;

		char **		_argv;
		char **		_env;
};

} /** namespace webserv */

#endif /** __CGI_HPP__ */
