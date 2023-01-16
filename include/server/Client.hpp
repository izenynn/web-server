#pragma once

#ifndef __CLIENT_HPP__
# define __CLIENT_HPP__

/** INCLUDES ----------------------------------- */

/** CLASS -------------------------------------- */

namespace webserv {

class Client {
	public:
		Client( const char* path ); // config class needs a config file
		virtual ~Client( void );
	private:
		Client( void ); // not necessary
		Client &operator=( const Client& other ); // not necessary
};

} /** namespace webserv */

#endif /** __CONFIG_HPP__ */
