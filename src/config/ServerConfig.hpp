#pragma once

#ifndef __SERVER_CONFIG_HPP__
# define __SERVER_CONFIG_HPP__

/** INCLUDES ----------------------------------- */

# include <string>
# include <vector>
# include <map>
# include <algorithm> // std::count()
# include <cstdio> // sscanf

# include <types/nullptr_t.hpp>
# include <config/Listen.hpp>

/** CLASS -------------------------------------- */

namespace webserv {

class ServerConfig {
	private:
		typedef std::vector<std::string> token_type;
	public:
		ServerConfig( void );
		virtual ~ServerConfig( void );

		void parser( token_type::const_iterator & it );

		const int & getId( void ) const;
		void setId( const int & id );
	private:
		ServerConfig( const ServerConfig & other ); // net necessary
		ServerConfig &operator=( const ServerConfig& other ); // not necessary

		int _id;

		typedef void ( ServerConfig::*parse_directive_type )( token_type::const_iterator & );
		std::map<std::string, parse_directive_type> _directivesValid;

		void parseLocation(				token_type::const_iterator & it );
		void parseListen(				token_type::const_iterator & it );
		void parseServerName(			token_type::const_iterator & it );
		void parseRoot(					token_type::const_iterator & it );
		void parseIndex(				token_type::const_iterator & it );
		void parseAutoindex(			token_type::const_iterator & it );
		void parseErrorPage(			token_type::const_iterator & it );
		void parseLimitExcept(			token_type::const_iterator & it );
		void parseClientMaxBodySize(	token_type::const_iterator & it );

		std::vector<Listen *>				_listens;

		/*std::vector<std::string>			_server_name;
		std::vector<std::string>			_allowed_methods;
		std::vector<std::string>			_index;
		std::map<std::string, ServerConfig>	_location;
		std::map<int, std::string>			_error_page;
		int									_client_max_body_size;
		bool								_autoindex;*/
	public:
		class ServerConfigException : virtual public std::exception {
			private:
				std::string message;
			public:
				ServerConfigException( const std::string & msg );
				~ServerConfigException( void ) throw ();
				const char * what() const throw ();
		};
};

} /** namespace webserv */

#endif /** __SERVER_CONFIG_HPP__ */
