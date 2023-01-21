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

		void print( const std::string & indent ); // FIXME just to test, remove this
		void parser( token_type::const_iterator & it );

		const int & getId( void ) const;
		void setId( const int & id );
	private:
		ServerConfig( const ServerConfig & other ); // not necessary
		ServerConfig & operator=( const ServerConfig & other ); // not necessary

		int _id;

		void			clear( void ); // FIXME not used
		ServerConfig *	createLocationServerConfig( void );

		typedef void ( ServerConfig::*parse_directive_type )( token_type::const_iterator & );
		std::map<std::string, parse_directive_type> _serverDirectives;
		std::map<std::string, parse_directive_type> _locationDirectives;

		void parseLocation(				token_type::const_iterator & it );
		void parseListen(				token_type::const_iterator & it );
		void parseServerName(			token_type::const_iterator & it );
		void parseRoot(					token_type::const_iterator & it );
		void parseIndex(				token_type::const_iterator & it );
		void parseAutoindex(			token_type::const_iterator & it );
		void parseErrorPage(			token_type::const_iterator & it );
		void parseLimitExcept(			token_type::const_iterator & it );
		void parseClientMaxBodySize(	token_type::const_iterator & it );

		std::vector<Listen *>					_listen;
		std::map<std::string, ServerConfig *>	_location;

		std::vector<std::string>			_server_name;
		std::string							_root;
		std::vector<std::string>			_index;
		bool								_autoindex;
		std::map<int, std::string>			_error_page;
		std::vector<std::string>			_limit_except;
		size_t								_client_max_body_size;
	public:
		class ServerConfigException : virtual public std::exception {
			private:
				std::string message;
			public:
				ServerConfigException( const std::string & msg );
				virtual ~ServerConfigException( void ) throw ();
				virtual const char * what( void ) const throw ();
		};
};

} /** namespace webserv */

#endif /** __SERVER_CONFIG_HPP__ */
