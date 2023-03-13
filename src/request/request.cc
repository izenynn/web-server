/** INCLUDES ----------------------------------- */

#include "request/request.h"

#include <cstdlib>

#include "webserv.h"

#include "config/config.h"

/** UTILS -------------------------------------- */

namespace {

bool isSupportedMethod( const std::string & method ) {
  bool ret = false;

  if ( "GET" == method ) ret = true;
  else if ( "POST" == method ) ret = true;
  else if ( "PUT" == method ) ret = true;
  else if ( "DELETE" == method ) ret = true;

  return ( ret );
}

bool isSupportedVersion( const std::string & version ) {
  bool ret = false;

  if ( "HTTP/1.1" == version ) ret = true;

  return ( ret );
}

bool isForbiddenUri( const std::string & uri ) {
  std::string path = uri;
  int cnt = 0;

  while ( std::string::npos != path.find( '/' ) ) {
    // take all except first '/' on each iteration
    // /images/bg.png
    //  ~~~~~~~~~~~~~
    path = path.substr( path.find( '/' ) + 1 );
    if ( true == path.empty() ) {
      break ;
    }

    // take folder/file name (in case no '/' find returns npos, so it takes the rest of the string)
    // images/bg.png
    // ~~~~~~
    std::string file = path.substr( 0, path.find( '/' ) );
    if ( std::string::npos != file.find( "..", 0 ) ) {
      --cnt;
    } else {
      ++cnt;
    }
  }

  if ( cnt < 0 ) {
    return ( true );
  }
  return ( false );
}

// trim from left
inline std::string & ltrim( std::string & s, const char * t = " \t\n\r\f\v" ) {
  std::string::size_type index = s.find_first_not_of( t );
  if ( std::string::npos == index ) {
    return ( s );
  }
  s.erase( 0, index );
  return ( s );
}

// trim from right
inline std::string & rtrim( std::string & s, const char * t = " \t\n\r\f\v" ) {
  std::string::size_type index = s.find_last_not_of( t );
  if ( std::string::npos == index ) {
    return ( s );
  }
  s.erase( index + 1 );
  return ( s );
}

// trim from left & right
inline std::string & trim( std::string & s, const char * t = " \t\n\r\f\v" ) {
  return ( ltrim( rtrim( s, t ), t ) );
}

template <typename T>
inline bool fromHex(const std::string& hexValue, T& result) {
  std::stringstream ss;
  ss << std::hex << hexValue;
  ss >> result;

  return ( !ss.fail() );
}

} /** namespace */

/** CLASS -------------------------------------- */

namespace webserv {

Request::Request( void ) {
  return ;
}

Request::~Request( void ) {
  return ;
}

void Request::print( void ) const {
  std::string i = "    ";
  std::cout << "\nREQUEST:" << std::endl;
  std::cout << i << "method:  " << this->_method << std::endl;
  std::cout << i << "uri:     " << this->_requestUri << std::endl;
  std::cout << i << "query:   " << this->_requestQuery << std::endl;
  std::cout << i << "version: " << this->_version << std::endl;

  std::cout << i << "headers:" << std::endl;
  for ( std::map<std::string, std::string>::const_iterator it = this->_headers.begin(); it != this->_headers.end(); ++it ) {
    std::cout << i << i << it->first << ": " << it->second << std::endl;
  }

  std::cout << i << "body:" << std::endl;
  std::cout << i << this->_body << std::endl;

  return ;
}

int Request::parse( const std::string & buffer ) {
  this->_buffer += buffer;

  int ret = this->parseRequestLine();
  if ( 0 == ret ) {
    ret = this->parseHeaders();
  }
  if ( 0 == ret ) {
    ret = this->parseBody();
  }

  return ( ret );
}

int Request::parseRequestLine( void ) {
  if ( std::string::npos != this->_buffer.find( kEOL ) ) {
    // method
    if ( std::string::npos == this->_buffer.find( ' ' ) || 0 == this->_buffer.find( ' ' ) ) {
      return ( 400 ); // 400 bad request
    }
    std::string token = this->_buffer.substr( 0, _buffer.find(' ') );
    if ( false == isSupportedMethod( token ) ) {
      return ( 501 ); // 501 not implemented
    }
    this->_method = token;
    this->_buffer.erase( 0, this->_method.length() + 1 );

    // uri
    if ( std::string::npos == this->_buffer.find( ' ' ) || 0 == this->_buffer.find( ' ' ) ) {
      return ( 400 ); // 400 bad request
    }
    token = this->_buffer.substr( 0, _buffer.find(' ') );
    if ( '/' != token[0] ) {
      return ( 400 ); // 400 bad request
    }
    if ( true == isForbiddenUri( token ) ) {
      return ( 403 ); // 403 forbidden
    }
    if ( token.length() >= kRequestUriLimit ) {
      return ( 414 ); // 414 uri too long
    }
    this->_requestUri = token;
    this->_buffer.erase( 0, this->_requestUri.length() + 1 );

    if ( std::string::npos != this->_requestUri.find( '?' ) ) {
      std::string::size_type start = this->_requestUri.find( '?' );
      this->_requestQuery = this->_requestUri.substr( start + 1 );
      this->_requestUri.erase( start );
    }

    // version
    if ( 0 == this->_buffer.find( ' ' ) ) {
      return ( 400 ); // 400 bad request
    }
    std::string::size_type last = this->_buffer.find( kEOL );
    token = this->_buffer.substr( 0, last );

    if ( false == isSupportedVersion( token ) ) {
      return ( 505 ); // 505 http version not supported
    }
    this->_version = token;
    this->_buffer.erase( 0, last + kEOL.length() );
  }
  return ( 0 );
}

int Request::parseHeaders( void ) {
  std::string key, value;
  std::string::size_type sep;

  // iterate lines until body
  for ( std::string::size_type eol = this->_buffer.find( kEOL ); eol != std::string::npos; eol = this->_buffer.find( kEOL ) ) {
    // if no more headers (two consecutive new lines)
    if ( 0 == eol ) {
      this->_buffer.erase( 0, eol + kEOL.length() );
      break ;
    }

    // parse headers
    sep = this->_buffer.find( ':' );
    if ( std::string::npos == sep || 0 == sep ) {
      return ( 400 ); // 400 bad request
    }
    key   = this->_buffer.substr( 0, sep );
    value = this->_buffer.substr( sep + 1, eol - sep - 1 );
    if ( this->_headers.end() != this->_headers.find( key ) ) {
      this->_buffer.erase( 0, eol + kEOL.length() );
      continue ;
    }
    if ( key.length() >= kRequestHeaderKeyLimit || value.length() >= kRequestHeaderValueLimit ) {
      return ( 431 ); // 431 request header fields too large
    }
    this->_headers[key] = trim( value, " " );
    if ( true == this->_headers[key].empty() ) {
      this->_headers.erase( key );
    }

    this->_buffer.erase( 0, eol + kEOL.length() );
  }

  // check host is present
  if ( this->_headers.end() == this->_headers.find( "Host" ) || this->_headers["Host"].empty() ) {
    return ( 400 ); // 400 bad request
  }

  return ( 0 );
}

int Request::parseBody( void ) {
  // chunked
  if ( this->_headers.end() != this->_headers.find( "Transfer-Encoding" ) && this->_headers["Transfer-Encoding"] == "chunked" ) {
    int ret = this->parseChunk();
    return ( ret );
  // not chunked
  } else if ( this->_headers.end() != this->_headers.find( "Content-Length" ) ) {
    if ( std::string::npos != this->_headers["Content-Length"].find_first_not_of( "0123456789" ) ) {
      return ( 400 ); // 400 bad request
    }
    long aux = atoi( this->_headers["Content-Length"].c_str() );
    if ( aux < 0 ) {
      return ( 400 ); // 400 bad request
    }
    this->_length = static_cast<std::string::size_type>(aux);
  // no body
  } else {
    return ( 0 );
  }

  // parse body
  if ( this->_buffer.length() >= this->_length ) {
    this->_body.insert( 0, this->_buffer, 0, this->_length );
    this->_buffer.clear();

    if ( this->_length != this->_body.length() ) {
      return ( 400 ); // 400 bad request
    }
  }

  return ( 0 );
}

int Request::parseChunk( void ) {
  bool readingChunkSize = true;
  std::string::size_type size = 0;

  for ( std::string::size_type eol = this->_buffer.find( kEOL ); eol != std::string::npos; eol = this->_buffer.find( kEOL ) ) {
    if ( true == readingChunkSize ) {
      std::string hex = this->_buffer.substr( 0, eol );
      if ( false == fromHex( hex, size ) ) {
        return ( 400 ); // 400 bad request
      }
      this->_buffer.erase( 0, eol + kEOL.length() );
      readingChunkSize = false;
    } else {
      if ( 0 == size ) {
        // trailer
        if ( false == this->_buffer.empty() ) {
          return ( this->parseChunkTrailer() );
        }
        return ( 0 );
      }
      this->_body += this->_buffer.substr( 0, size );
      this->_buffer.erase( 0, size + kEOL.length() );
      size = 0;
      readingChunkSize = true;
    }
  }

  return ( 0 );
}

int Request::parseChunkTrailer( void ) {
  std::string key, value;
  std::string::size_type sep;

  for ( std::string::size_type eol = this->_buffer.find( kEOL ); eol != std::string::npos; eol = this->_buffer.find( kEOL ) ) {
    // chunk trailer ends on line with only "\r\n"
    if ( 0 == this->_buffer.find( kEOL ) ) {
      this->_buffer.erase( 0, eol + kEOL.length() );
      break ;
    }
    // parse trailer
    sep = this->_buffer.find( ':' );
    if ( std::string::npos == sep || 0 == sep || this->_buffer[eol - 1] == ' ' ) {
      return ( 400 ); // 400 bad request
    }
    key   = this->_buffer.substr( 0, sep );
    value = this->_buffer.substr( sep + 1, eol - sep - 1 );
    if ( this->_headers.end() == this->_headers.find( key ) ) {
      if ( key.length() >= kRequestHeaderKeyLimit || value.length() >= kRequestHeaderValueLimit ) {
        return ( 400 ); // 400 bad request
      }
      this->_headers[key] = trim( value, " " );
      if ( true == this->_headers[key].empty() ) {
        this->_headers.erase( key );
      }
    }
    this->_buffer.erase( 0, eol + kEOL.length() );
  }

  return ( 0 );
}

} /** namespace webserv */
