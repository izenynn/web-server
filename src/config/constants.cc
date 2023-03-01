/** INCLUDES ----------------------------------- */

# include <config/constants.h>

/** METHODS ------------------------------------ */

namespace webserv {

const char *            kDefaultPath = "/etc/webserv/webserv.conf";
const std::string       kDefaultRoot = "/var/www/";
const int               kBacklogSize = 1024; // listen() argument: the backlog argument defines the maximum length to which the queue of pending connections for a sockfd may grow
const int               kBufferSize = 65536;
const time_t            kTimeoutSec = 1; // select() timeout in seconds
const long              kNsecLoopDelay = 500L * 1000L;
const std::string       kEOL = "\r\n";
const std::string::size_type  kRequestUriLimit = 8190; // nginx default
const std::string::size_type  kRequestHeaderKeyLimit = 1024; // nginx default
const std::string::size_type  kRequestHeaderValueLimit = 4096; // nginx default
const size_t            kClientMaxBodySize = 8196; // nginx default
const std::string       kDefaultVersion = "HTTP/1.1";
const std::string       kCgiTmpFile = "/tmp/webserv_cgi";
const size_t            kReadBuffer = 8192;

const std::map<int, Client *>::size_type kMaxClients = 1024;

} /** namespace webserv */
