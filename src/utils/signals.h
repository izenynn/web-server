#ifndef WEBSERV_SIGNALS_H_
# define WEBSERV_SIGNALS_H_

/** INCLUDES ----------------------------------- */

# include <csignal>

# include "server/server.h"

/** METHODS ------------------------------------ */

namespace webserv {

namespace signals {

void handle_signals( void );

} // namespace utils

} // namespace webserv

#endif // WEBSERV_SIGNALS_H_
