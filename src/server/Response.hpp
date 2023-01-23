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
		Response( const RequestConfig & config, int status_code );
		~Response( void );
	private:
		Response( void ); // not necessary
		Response( const Response & other ); // not necessary
		Response & operator=( const Response & other ); // not necessary

		static std::map<int, std::string>			_status_codes;
		static std::map<std::string, std::string>	_mime_types;

		static std::map<int, std::string>			initStatusCodes( void );
		static std::map<std::string, std::string>	initMimeTypes( void );
};

} /** namespace webserv */

#endif /** __RESPONSE_HPP__ */
