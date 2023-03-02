#ifndef WEBSERV_RESPONSE_H_
# define WEBSERV_RESPONSE_H_

/** INCLUDES ----------------------------------- */

# include <map>
# include <string>
# include <iostream>

# include "request/request_data.h"
# include "response/response_data.h"

/** CLASS -------------------------------------- */

namespace webserv {

class RequestData;
class ResponseData;

class Response {
  public:
    Response( RequestData & requestData, int statusCode );
    ~Response( void );

    void print( void ) const;

    void clear( void );
    void build( void );

    const std::string & getResponseBody( void );
    bool getRedirect( void );
    std::string getRedirectUri( void );

    bool isConnectionClose( void );
  private:
    Response( void ); // not necessary
    Response( const Response & other ); // not necessary
    Response & operator=( const Response & other ); // not necessary

    static std::map<int, std::string>     kStatusCodes;
    static std::map<std::string, std::string> kMimeTypes;

    static std::map<int, std::string>     initStatusCodes( void );
    static std::map<std::string, std::string> initMimeTypes( void );

    typedef int ( Response::*method )( void );

    int   process( void );
    void  setResponse( void );

    int methodGet( void );
    int methodPost( void );
    int methodPut( void );
    int methodDelete( void );
    std::map<std::string, Response::method> _methods;

    void generateErrorPage( const int statusCode );
    void generateReturnPage( void );
    void generateRedirectPage( const int statusCode, const std::string & uri );

    bool      _redirect;
    std::string   _redirect_uri;
    int       _redirect_status_code;

    std::string             _response;

    int                 _statusCode;
    RequestData &           _requestData;
    std::map<std::string, std::string>  _headers;
    std::string             _body;

    ResponseData            _responseData;
};

} // namespace webserv

#endif // WEBSERV_RESPONSE_H_
