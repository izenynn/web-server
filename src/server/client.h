#ifndef WEBSERV_CLIENT_H_
# define WEBSERV_CLIENT_H_

/** INCLUDES ----------------------------------- */

# include <unistd.h>
# include <sys/time.h>
# include <sys/socket.h>

# include <vector>

/** CLASS -------------------------------------- */

namespace webserv {

struct Listen;
class ServerConfig;
class Request;
class RequestData;
class Response;

class Client {
  public:
    Client( int fd, Listen & host );
    ~Client( void );

    void clear( void );

    void initRequest( void );
    void initResponse( const std::vector<ServerConfig *> & servers, int statusCode );
    void initRequestData( const std::vector<ServerConfig *> & servers );


    int       getFd( void ) const;
    Request *   getRequest( void ) const;
    Response *    getResponse( void ) const;
    RequestData * getRequestData( void ) const;
    bool      getDisconnect( void ) const;

    void      setDisconnct( bool value );
  private:
    Client( void ); // not necessary
    Client( const Client & other ); // not necessary
    Client & operator=( const Client & other ); // not necessary

    int       _fd;
    Listen &    _host;
    bool      _disconnect;

    Request *   _request;
    Response *    _response;
    RequestData * _requestData;
};

} // namespace webserv

#endif // WEBSERV_CLIENT_H_
