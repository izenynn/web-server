#pragma once

#ifndef __LISTEN_HPP__
# define __LISTEN_HPP__

/** INCLUDES ----------------------------------- */

# include <stdint.h>

/** CLASS -------------------------------------- */

namespace webserv {

struct Listen {
	uint32_t host;
	uint16_t port;

	Listen() : host(0), port(0) {};
	Listen(uint32_t host, uint16_t port) : host(host), port(port) {}; // FIXME is this legal? variables with same name as args?
};

} /** namespace webserv */

#endif /** __LISTEN_HPP__ */
