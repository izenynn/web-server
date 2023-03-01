#pragma once

#ifndef __REQUEST_HPP__
# define __REQUEST_HPP__

/** INCLUDES ----------------------------------- */

# include <string>
# include <map>
# include <sstream>
# include <sys/time.h>

# include <config/Config.hpp>

/** CLASS -------------------------------------- */

namespace webserv {

class Request {
  public:
    Request( void );
    ~Request( void );

    void print( void ) const;

    int parse( const std::string & buffer );

    friend class RequestData;
  private:
    Request( const Request & other ); // not necessary
    Request & operator=( const Request & other ); // not necessary

    int parseRequestLine( void );
    int parseHeaders( void );
    int parseBody( void );
    int parseChunk( void );

    int parseChunkTrailer( void );

    std::string   _method;
    std::string   _requestUri;
    std::string   _requestQuery;
    std::string   _version;
    std::string   _body;
    std::map<std::string, std::string> _headers;

    std::string   _buffer;

    std::string::size_type  _length;
};

} /** namespace webserv */

#endif /** __REQUEST_HPP__ */
