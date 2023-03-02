#ifndef WEBSERV_UTILS_H_
# define WEBSERV_UTILS_H_

/** INCLUDES ----------------------------------- */

# include <string>

/** METHODS ------------------------------------ */

namespace webserv {

namespace utils {

std::string sanitizePath( std::string path );

} // namespace utils

} // namespace webserv

#endif // WEBSERV_UTILS_H_
