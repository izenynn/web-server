#pragma once

#ifndef __CONSTANTS_HPP__
# define __CONSTANTS_HPP__

/** INCLUDES ----------------------------------- */

# include <string>
# include <ctime>

/** METHODS ------------------------------------ */

namespace webserv {
	const char *					kDefaultPath = "/etc/web-serv/web-serv.conf";
	const int						kBacklogSize = 1024; // listen() argument: the backlog argument defines the maximum length to which the queue of pending connections for a sockfd may grow
	const int						kBufferSize = 16384;
	const time_t					kTimeoutSec = 1; // select() timeout in seconds
	const long						kNsecLoopDelay = 500L * 1000L;
	const std::string				kEOL = "\r\n";
	const std::string::size_type	kLimitRequestLimit = 8190; // nginx default
	const time_t					kRequestTimeoutSec = 60; // nginx default
} /** namespace webserv */

#endif /** __CONSTANTS_HPP__ */
