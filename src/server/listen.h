#ifndef WEBSERV_LISTEN_H_
# define WEBSERV_LISTEN_H_

/** INCLUDES ----------------------------------- */

# include <stdint.h>

# include <string>

/** CLASS -------------------------------------- */

namespace webserv {

struct Listen {
  std::string _ip;
  uint16_t _port;

  Listen() : _ip(""), _port(0) {};
  Listen(std::string ip, uint16_t port) : _ip(ip), _port(port) {};
};

inline bool operator==( const Listen & lhs, const Listen & rhs ) {
  return ( lhs._ip == rhs._ip && lhs._port == rhs._port );
}

} // namespace webserv

#endif // WEBSERV_LISTEN_H_
