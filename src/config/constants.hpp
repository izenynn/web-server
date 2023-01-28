#pragma once

#ifndef __CONSTANTS_HPP__
# define __CONSTANTS_HPP__

/** INCLUDES ----------------------------------- */

# include <string>
# include <map>
# include <ctime>

/** METHODS ------------------------------------ */

namespace webserv {
	class Client;

	extern const char *					kDefaultPath;
	extern const int					kBacklogSize;
	extern const int					kBufferSize;
	extern const time_t					kTimeoutSec;
	extern const long					kNsecLoopDelay;
	extern const std::string			kEOL;
	extern const std::string::size_type	kRequestUriLimit;
	extern const std::string::size_type	kRequestHeaderKeyLimit;
	extern const std::string::size_type	kRequestHeaderValueLimit;
	extern const time_t					kRequestTimeoutSec;
	extern const size_t					kClientMaxBodySize;

	extern const std::map<int, Client *>::size_type kMaxClients;
} /** namespace webserv */

#endif /** __CONSTANTS_HPP__ */
