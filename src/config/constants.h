#ifndef WEBSERV_CONSTANTS_H_
# define WEBSERV_CONSTANTS_H_

/** INCLUDES ----------------------------------- */

# include <string>
# include <map>
# include <ctime>

/** METHODS ------------------------------------ */

namespace webserv {

class Client;

extern const char *           kDefaultPath;
extern const std::string      kDefaultRoot;
extern const int              kBacklogSize;
extern const unsigned long    kBufferSize;
extern const time_t           kTimeoutSec;
extern const long             kNsecLoopDelay;
extern const std::string      kEOL;
extern const std::string::size_type kRequestUriLimit;
extern const std::string::size_type kRequestHeaderKeyLimit;
extern const std::string::size_type kRequestHeaderValueLimit;
extern const size_t           kClientMaxBodySize;
extern const std::string      kDefaultVersion;
extern const std::string      kCgiTmpFile;
extern const size_t           kReadBuffer;

extern const std::map<int, Client *>::size_type kMaxClients;

} // namespace webserv

#endif // WEBSERV_CONSTANTS_H_
