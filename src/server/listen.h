#ifndef WEBSERV_LISTEN_H_
# define WEBSERV_LISTEN_H_

/** INCLUDES ----------------------------------- */

# include <string>
# include <stdint.h>

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

} // namespace webserv

#endif // WEBSERV_LISTEN_H_
