#pragma once

#ifndef __SERVER_CONFIG_HPP__
# define __SERVER_CONFIG_HPP__

/** INCLUDES ----------------------------------- */

# include <string>
# include <vector>

# include <types/nullptr_t.hpp>
# include <server/Server.hpp>

/** CLASS -------------------------------------- */

namespace webserv {

class ServerConfig {
	public:
		ServerConfig( void );
		virtual ~ServerConfig( void );

		int parse( const std::vector<std::string> * const tokens, std::vector<std::string>::const_iterator & it );
	private:
		ServerConfig( const ServerConfig & other ); // net necessary
		ServerConfig &operator=( const ServerConfig& other ); // not necessary

		std::vector<listen_t>				_listen;
		
		std::vector<std::string>			_server_name;
		std::vector<std::string>			_allowed_methods;
		std::vector<std::string>			_index;
		std::map<std::string, ServerConfig>	_location;
		std::map<int, std::string>			_error_page;
		int									_client_max_body_size;
		bool								_autoindex;
};

} /** namespace webserv */

#endif /** __SERVER_CONFIG_HPP__ */
