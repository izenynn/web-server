#ifndef WEBSERV_REQUESTDATA_H_
# define WEBSERV_REQUESTDATA_H_

/** INCLUDES ----------------------------------- */

# include <vector>
# include <string>
# include <stdint.h>

# include "request/request.h"

/** CLASS -------------------------------------- */

namespace webserv {

class Listen;
class Client;
class ServerConfig;

class RequestData {
  public:
    RequestData( Request & request, Listen & host, Client & client, const std::vector<ServerConfig *> & servers );
    ~RequestData( void );

    void print( void ) const;

    void initialize( void );
    void redirect( const std::string & uri );

    bool isValidMethod( const std::string & method );

    // request related getters
    const std::string &         getMethod( void ) const;
    const std::string &         getBody( void ) const;
    const std::string &         getRequestUri( void ) const;
    const std::string &         getRequestQuery( void ) const;
    const std::string &         getRequestRequestUri( void ) const;
    const std::string &         getVersion( void ) const;
    const std::string &         getHost( void ) const;
    uint16_t              getPort( void ) const;
    const std::map<std::string, std::string> & getHeaders( void ) const;

    void                setMethod( const std::string & value ) const;

    // server/location related getters
    const std::map<int, std::string> &  getErrorPages( void ) const;
    const std::vector<std::string> &  getAllowedMethods( void ) const;
    std::string::size_type        getMaxBodySize( void ) const;
    const std::vector<std::string> &  getIndex( void ) const;
    bool                getAutoIndex( void ) const;
    const std::string &         getLocationUri( void ) const;
    const std::string &         getRoot( void ) const;
    const std::string &         getUploadStore( void ) const;
    const std::string &         getAlias( void ) const;
    const std::pair<int, std::string> & getReturn( void ) const;
    const std::map<std::string, std::string> & getCgi( void ) const;
  private:
    RequestData( void ); // not necessary
    RequestData( const RequestData & other ); // not necessary
    RequestData & operator=( const RequestData & other ); // not necessary

    ServerConfig *                      getRequestServer( void );
    const std::pair<const std::string, ServerConfig *> *  getRequestLocation( const ServerConfig * const server );

    std::string             _request_uri;

    Request &             _request;
    Listen &              _host;
    Client &              _client;

    const std::vector<ServerConfig *> & _servers;

    const ServerConfig *                  _server;
    const std::pair<const std::string, ServerConfig *> *  _location;
    bool                          _locationAllocated;
};

} // namespace webserv

#endif // WEBSERV_REQUESTDATA_H_
