#pragma once

#ifndef __LISTEN_HPP__
# define __LISTEN_HPP__

/** INCLUDES ----------------------------------- */

# include <stdint.h>
# include <string>

/** CLASS -------------------------------------- */

namespace webserv {

struct Listen {
	//uint32_t host;
	std::string	ip;
	uint16_t	port;

	//Listen() : host(0), port(0) {};
	//Listen(uint32_t host, uint16_t port) : host(host), port(port) {}; // FIXME is this legal? variables with same name as args?
	Listen() : ip(""), port(0) {};
	Listen(std::string ip, uint16_t port) : ip(ip), port(port) {}; // FIXME is this legal? variables with same name as args?
};

inline bool operator==( const Listen & lhs, const Listen & rhs ) {
	return ( lhs.ip == rhs.ip && lhs.port == rhs.port );
}

} /** namespace webserv */

#endif /** __LISTEN_HPP__ */
