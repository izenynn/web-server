#pragma once

#ifndef __UTILS_HPP__
# define __UTILS_HPP__

/** INCLUDES ----------------------------------- */

# include <string>

/** METHODS ------------------------------------ */

namespace webserv {

namespace utils {

std::string sanitizePath( std::string path );

} /** namespace utils */

} /** namespace webserv */

#endif /** __UTILS_HPP__ */
