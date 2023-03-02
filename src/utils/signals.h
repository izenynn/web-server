#pragma once

#ifndef __SIGNALS_HPP__
# define __SIGNALS_HPP__

/** INCLUDES ----------------------------------- */

# include <csignal>

# include <server/server.h>

/** METHODS ------------------------------------ */

namespace webserv {

namespace signals {

void handle_signals( void );

} // namespace utils

} // namespace webserv

#endif // __SIGNALS_HPP__
