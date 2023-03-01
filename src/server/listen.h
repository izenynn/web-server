#pragma once

#ifndef __LISTEN_HPP__
# define __LISTEN_HPP__

/** INCLUDES ----------------------------------- */

# include <stdint.h>
# include <string>

/** CLASS -------------------------------------- */

namespace webserv {

struct Listen {
  std::string ip;
  uint16_t  port;

  Listen() : ip(""), port(0) {};
  Listen(std::string ip, uint16_t port) : ip(ip), port(port) {};
};

inline bool operator==( const Listen & lhs, const Listen & rhs ) {
  return ( lhs.ip == rhs.ip && lhs.port == rhs.port );
}

} /** namespace webserv */

#endif /** __LISTEN_HPP__ */
