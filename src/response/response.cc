/** INCLUDES ----------------------------------- */

#include <response/response.h>

#include <config/constants.h>
#include <utils/utils.h>
#include <utils/log.h>

/** UTILS -------------------------------------- */

namespace {

std::string getDate( void ) {
  struct timeval time;
  char buffer[128];

  gettimeofday( &time, NULL );

  struct tm *htime;
  htime = gmtime( &time.tv_sec );

  // Example: Date: Tue, 24 Dec 2022 19:22:21 GMT
  //int length = strftime( buffer, 32, "%a, %d %b %Y %T GMT", htime );
  size_t length = strftime( buffer, 32, "%a, %d %b %Y %H:%M:%S GMT", htime );

  return ( std::string( buffer, length ) );
}

std::string removeLocationFromUri( std::string uri, const std::string & locationUri ) {
  // check request is not the base directory
  uri.erase( 0, locationUri.length() );

  // put '/' at the start if it was removed
  if ( '/' != uri[0] ) {
    uri.insert( 0, "/" );
  }

  return ( uri );
}

}

/** CLASS -------------------------------------- */

namespace webserv {

std::map<int, std::string> Response::initStatusCodes() {
  std::map<int, std::string> status_codes;

  status_codes[100] = "Continue";
  status_codes[101] = "Switching Protocols";
  status_codes[102] = "Procossing";
  status_codes[103] = "Early Hints";

  status_codes[200] = "OK";
  status_codes[201] = "Created";
  status_codes[202] = "Accepted";
  status_codes[203] = "Non-Authoritative Information";
  status_codes[204] = "No Content";
  status_codes[205] = "Reset Content";
  status_codes[206] = "Partial Content";
  status_codes[207] = "Multi-Status";
  status_codes[208] = "Already Reported";
  status_codes[226] = "IM Used";

  status_codes[300] = "Multiple Choices";
  status_codes[301] = "Moved Permanently";
  status_codes[302] = "Found";
  status_codes[303] = "See Other";
  status_codes[304] = "Not Modified";
  status_codes[305] = "Use Proxy";
  status_codes[306] = "unused";
  status_codes[307] = "Temporary Redirect";
  status_codes[308] = "Permanent Redirect";

  status_codes[400] = "Bad Request";
  status_codes[401] = "Unauthorized";
  status_codes[402] = "Payment Required";
  status_codes[403] = "Forbidden";
  status_codes[404] = "Not Found";
  status_codes[405] = "Method Not Allowed";
  status_codes[406] = "Not Acceptable";
  status_codes[407] = "Proxy Authentication Required";
  status_codes[408] = "Request Timeout";
  status_codes[409] = "Conflict";
  status_codes[410] = "Gone";
  status_codes[411] = "Length Required";
  status_codes[412] = "Precondition Failed";
  status_codes[413] = "Payload Too Large";
  status_codes[414] = "URI Too Long";
  status_codes[415] = "Unsupported Media Type";
  status_codes[416] = "Range Not Satisfiable";
  status_codes[417] = "Expectation Failed";
  status_codes[418] = "I'm a teapot";
  status_codes[421] = "Misdirected Request";
  status_codes[422] = "Unprocessable Entity";
  status_codes[423] = "Locked";
  status_codes[424] = "Failed Dependency";
  status_codes[425] = "Too Early";
  status_codes[426] = "Upgrade Required";
  status_codes[428] = "Precondition Required";
  status_codes[429] = "Too Many Requests";
  status_codes[431] = "Request Header Fields Too Large";
  status_codes[451] = "Unavailable For Legal Reasons";

  status_codes[500] = "Internal Server Error";
  status_codes[501] = "Not Implemented";
  status_codes[502] = "Bad Gateway";
  status_codes[503] = "Service Unavailable";
  status_codes[504] = "Gateway Timeout";
  status_codes[505] = "HTTP Version Not Supported";
  status_codes[506] = "Variant Also Negotiates";
  status_codes[507] = "Insufficient Staroge";
  status_codes[508] = "Loop Detected";
  status_codes[510] = "Not Extended";
  status_codes[511] = "Network Authentication Required";

  return ( status_codes );
}

std::map<std::string, std::string> Response::initMimeTypes( void ) {
  std::map<std::string, std::string> mime_types;

  mime_types[".aac"]    = "audio/aac";
  mime_types[".abw"]    = "application/x-abiword";
  mime_types[".arc"]    = "application/x-freearc";
  mime_types[".avif"]   = "image/avif";
  mime_types[".avi"]    = "video/x-msvideo";
  mime_types[".azw"]    = "application/vnd.amazon.ebook";
  mime_types[".bin"]    = "application/octet-stream";
  mime_types[".bmp"]    = "image/bmp";
  mime_types[".bz"]   = "application/x-bzip";
  mime_types[".bz2"]    = "application/x-bzip2";
  mime_types[".cda"]    = "application/x-cdf";
  mime_types[".csh"]    = "application/x-csh";
  mime_types[".css"]    = "text/css";
  mime_types[".csv"]    = "text/csv";
  mime_types[".doc"]    = "application/msword";
  mime_types[".docx"]   = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
  mime_types[".eot"]    = "application/vnd.ms-fontobject";
  mime_types[".epub"]   = "application/epub+zip";
  mime_types[".gz"]   = "application/gzip";
  mime_types[".gif"]    = "image/gif";
  mime_types[".htm"]    = "text/html";
  mime_types[".html"]   = "text/html";
  mime_types[".ico"]    = "image/vnd.microsoft.icon";
  mime_types[".ics"]    = "text/calendar";
  mime_types[".jar"]    = "application/java-archive";
  mime_types[".jpeg"]   = "image/jpeg";
  mime_types[".jpg"]    = "image/jpeg";
  mime_types[".js"]   = "text/javascript";
  mime_types[".json"]   = "application/json";
  mime_types[".jsonld"] = "application/ld+json";
  mime_types[".mid"]    = "audio/midi";
  mime_types[".midi"]   = "audio/midi";
  mime_types[".mjs"]    = "text/javascript";
  mime_types[".mp3"]    = "audio/mpeg";
  mime_types[".mp4"]    = "video/mp4";
  mime_types[".mpeg"]   = "video/mpeg";
  mime_types[".mpkg"]   = "application/vnd.apple.installer+xml";
  mime_types[".odp"]    = "application/vnd.oasis.opendocument.presentation";
  mime_types[".ods"]    = "application/vnd.oasis.opendocument.spreadsheet";
  mime_types[".odt"]    = "application/vnd.oasis.opendocument.text";
  mime_types[".oga"]    = "audio/ogg";
  mime_types[".ogv"]    = "video/ogg";
  mime_types[".ogx"]    = "application/ogg";
  mime_types[".opus"]   = "audio/opus";
  mime_types[".otf"]    = "font/otf";
  mime_types[".png"]    = "image/png";
  mime_types[".pdf"]    = "application/pdf";
  mime_types[".php"]    = "application/x-httpd-php";
  mime_types[".ppt"]    = "application/vnd.ms-powerpoint";
  mime_types[".pptx"]   = "application/vnd.openxmlformats-officedocument.presentationml.presentation";
  mime_types[".rar"]    = "application/vnd.rar";
  mime_types[".rtf"]    = "application/rtf";
  mime_types[".sh"]   = "application/x-sh";
  mime_types[".svg"]    = "image/svg+xml";
  mime_types[".tar"]    = "application/x-tar";
  mime_types[".tif"]    = "image/tiff";
  mime_types[".tiff"]   = "image/tiff";
  mime_types[".ts"]   = "video/mp2t";
  mime_types[".ttf"]    = "font/ttf";
  mime_types[".txt"]    = "text/plain";
  mime_types[".vsd"]    = "application/vnd.visio";
  mime_types[".wav"]    = "audio/wav";
  mime_types[".weba"]   = "audio/webm";
  mime_types[".webm"]   = "video/webm";
  mime_types[".webp"]   = "image/webp";
  mime_types[".woff"]   = "font/woff";
  mime_types[".woff2"]  = "font/woff2";
  mime_types[".xhtml"]  = "application/xhtml+xml";
  mime_types[".xls"]    = "application/vnd.ms-excel";
  mime_types[".xlsx"]   = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
  mime_types[".xml"]    = "application/xml";
  mime_types[".xul"]    = "application/vnd.mozilla.xul+xml";
  mime_types[".zip"]    = "application/zip";
  mime_types[".3gp"]    = "video/3gpp";
  mime_types[".3g2"]    = "video/3gpp2";
  mime_types[".7z"]   = "application/x-7z-compressed";

  return ( mime_types );
}

std::map<int, std::string>      Response::kStatusCodes = initStatusCodes();
std::map<std::string, std::string>  Response::kMimeTypes = initMimeTypes();

Response::Response( RequestData & requestData, int statusCode )
    : _redirect( false ),
      _redirect_status_code( 0 ),
      _statusCode( statusCode ),
      _requestData( requestData ) {
  this->_methods["GET"]   = &Response::methodGet;
  this->_methods["POST"]    = &Response::methodPost;
  this->_methods["PUT"]   = &Response::methodPut;
  this->_methods["DELETE"]  = &Response::methodDelete;
  return ;
}

Response::~Response( void ) {
  return ;
}

void Response::print( void ) const {
  std::string i = "    ";
  std::cout << "\nRESPONSE:" << std::endl;

  std::cout << i << "redirect:     " << std::boolalpha << this->_redirect << std::endl;
  std::cout << i << "redirect uri: " << this->_redirect_uri << std::endl;
  std::cout << i << "redirect st:  " << this->_redirect_status_code << std::endl;
  std::cout << i << "status code:  " << this->_statusCode << std::endl;
  std::cout << i << "headers:" << std::endl;
  for ( std::map<std::string, std::string>::const_iterator it = this->_headers.begin(); it != this->_headers.end(); ++it ) {
    std::cout << i << i << it->first << ": " << it->second << std::endl;
  }

  std::cout << i << "body:" << std::endl;
  std::cout << i << this->_body << std::endl;

  std::cout << i << "response:" << std::endl;
  std::cout << i << this->_response << std::endl;

  std::cout << i << "request config:" << std::endl;
  this->_requestData.print();

  std::cout << i << "uri:" << std::endl;
  this->_responseData.print();

  return ;
}

void Response::clear( void ) {
  this->_redirect = false;
  this->_statusCode = 0;
  this->_headers.clear();
  this->_body.clear();
  this->_response.clear();
}

void Response::build( void ) {
  const std::string & method = this->_requestData.getMethod();

  // check if return directive
  if ( 0 != this->_requestData.getReturn().first ) {
    this->generateReturnPage();
    this->setResponse();
    return ;
  }

  // set path depending if 'alias' directive is present on location
  if ( false == this->_requestData.getAlias().empty() ) {
    this->_responseData.setPath( this->_requestData.getAlias() + "/" + removeLocationFromUri( this->_requestData.getRequestUri(), this->_requestData.getLocationUri() ) );
  } else {
    this->_responseData.setPath( this->_requestData.getRoot() + "/" + this->_requestData.getRequestUri() );
  }

  // check if redirect, check for errors and process request if none
  if ( 0 == this->_statusCode ) {
    // if is directory redirect to same uri but append a '/' at the end
    const std::string uri = this->_requestData.getRequestUri();
    if ( true == this->_responseData.isDirectory() && '/' != uri[uri.length() - 1] ) {
      this->generateRedirectPage( 301, uri + "/" );
      this->setResponse();
      return ;
    } else if ( false == this->_requestData.isValidMethod( method ) ) {
      // set error code
      this->_statusCode = 405; // 405 method not allowed
      // set allow header
      std::string allowedMethods;
      for ( std::vector<std::string>::const_iterator it = this->_requestData.getAllowedMethods().begin(); it != this->_requestData.getAllowedMethods().end(); ) {
        allowedMethods += *it;
        ++it;
        if ( this->_requestData.getAllowedMethods().end() != it ) {
          allowedMethods += ", ";
        }
      }
      this->_headers["Allow"] = allowedMethods;
    } else if ( this->_requestData.getBody().length() > this->_requestData.getMaxBodySize() ) {
      this->_statusCode = 413; // 413 payload too large
    } else {
      this->_statusCode = this->process();
    }
  }

  // check if error page needed
  if ( this->_statusCode >= 300 && 0 == this->_body.length() ) {
    this->generateErrorPage( this->_statusCode );
  }

  // check if not redirection
  if ( false == this->_redirect ) {
    this->setResponse();
  }

  return ;
}

const std::string & Response::getResponseBody( void ) {
  return ( this->_response );
}

bool Response::getRedirect( void ) {
  return ( this->_redirect );
}

std::string Response::getRedirectUri( void ) {
  return ( this->_redirect_uri );
}

bool Response::isConnectionClose( void ) {
  if ( this->_headers.end() != this->_headers.find( "Connection" ) ) {
    if ( "close" == this->_headers["Connection"] ) {
      return ( true );
    }
  }
  return ( false );
}

int Response::process( void ) {
  const std::string & method = this->_requestData.getMethod();

  // get -> checks and internal redirect (index/autoindex directive)
  if ( "GET" == method ) {
    // directory, if index -> go to index, else if no index and no autoindex -> bad request
    if ( true == this->_responseData.isDirectory() ) {
      const std::string & index = this->_responseData.getIndex( this->_requestData.getIndex() );
      if ( index.length() > 0 ) {
        this->_redirect = true;
        this->_redirect_uri = utils::sanitizePath( "/" + this->_requestData.getRequestUri() + "/" + index );
        return ( 200 ); // 200 ok
      } else if ( false == this->_requestData.getAutoIndex() ) {
        return ( 404 ); // 404 bad request
      }
    // not directory
    } else {
      if ( false == this->_responseData.fileExists() ) {
        return ( 404 ); // 404 bad request
      }

      if ( false == this->_responseData.openFile() ) {
        return ( 403 ); // 403 forbidden
      }
    }

  }

  // cgi -> check and execute on match
  for ( std::map<std::string, std::string>::const_iterator it = this->_requestData.getCgi().begin(); it != this->_requestData.getCgi().end(); ++it ) {
    if ( this->_responseData.getExtension() == it->first ) {
      Cgi * cgi = new Cgi( this->_requestData, this->_responseData );

      int ret = cgi->exec();
      if ( ret >= 400 ) {
        delete cgi;
        this->_statusCode = ret;
        return ( this->_statusCode );
      }

      cgi->getHeadersAndBody( this->_headers, this->_body );
      this->_headers["Content-Length"] = SSTR( this->_body.length() );

      delete cgi;
      this->_statusCode = ret;
      return ( this->_statusCode );
    }
  }

  // post / put
  if ( "POST" == method || "PUT" == method ) {
    // check request is not the base directory
    {
      std::string location = utils::sanitizePath( this->_requestData.getLocationUri() );
      std::string request = utils::sanitizePath( this->_requestData.getRequestUri() );
      if ( '/' == location[location.length() - 1] ) {
        location.erase( location.length() - 1 );
      }
      if ( '/' == request[request.length() - 1] ) {
        request.erase( request.length() - 1 );
      }
      if ( request == location ) {
        return ( 405 ); // 405 method not allowed
      }
    }

    // if upload_store change set upload path
    if ( false == this->_requestData.getUploadStore().empty() ) {
      std::string uploadPath = this->_requestData.getRoot() + "/" + this->_requestData.getUploadStore();

      bool fileExists = false;
      bool isDir = false;
      struct stat statbuf;
      if ( 0 == stat( uploadPath.c_str(), &statbuf ) ) {
        fileExists = true;
      }
      if ( S_ISDIR( statbuf.st_mode ) ) {
        isDir = true;
      }

      if ( false == fileExists || false == isDir ) {
        LOG_FAILURE( "POST/PUT failed because upload path: " << uploadPath << " does not exists or is not a directory" );
        return ( 500 ); // 500 internal server error
      }

      // romove location uri from request uri
      std::string file;
      if ( 0 == uploadPath.find( this->_requestData.getRoot() ) ) {
        // remove '/' at the end, it shouldn't have, that would mean it's a dir ande we check that before, but just in case to avoid errors
        std::string request = this->_requestData.getRequestUri();
        if ( '/' == request[request.length() - 1] ) {
          request.erase( request.length() - 1 );
        }
        file = request.substr( request.find_last_of( "/" ), request.npos );
      }
      if ( true == file.empty() ) {
        LOG_FAILURE( "unexpected error on POST/PUT, no file specified on request uri: " << this->_requestData.getRequestUri() << " does not exists or is not a directory" );
        return ( 400 ); // 400 bad request
      }
      this->_responseData.setPath( uploadPath + "/" + file );
      //this->_responseData.setPath( uploadPath + "/" + this->_requestData.getRequestUri() );
    }
  }

  // delete
  if ( "DELETE" == method ) {
    // check request is not the base directory
    std::string location = utils::sanitizePath( this->_requestData.getLocationUri() );
    std::string request = utils::sanitizePath( this->_requestData.getRequestUri() );
    if ( '/' == location[location.length() - 1] ) {
      location.erase( location.length() - 1 );
    }
    if ( '/' == request[request.length() - 1] ) {
      request.erase( request.length() - 1 );
    }
    if ( request == location ) {
      return ( 405 ); // 405 method not allowed
    }
  }

  int ret = (this->*(Response::_methods[method]))();
  return ( ret );
}

void Response::setResponse( void ) {
  if ( this->_statusCode < 400 && this->_redirect_status_code != 0 ) {
    this->_statusCode = this->_redirect_status_code;
    this->_redirect_status_code = 0;
  }

  std::string status;
  if ( this->_headers.end() != this->_headers.find( "Status" ) ) {
    std::map<std::string, std::string>::iterator it = this->_headers.find( "Status" );
    status = it->second;
    this->_headers.erase( it );
  } else {
    status = SSTR( this->_statusCode ) + " " + this->kStatusCodes[this->_statusCode];
  }
  this->_response += this->_requestData.getVersion() + " " + status + kEOL;

  //this->_headers["Server"] = "web-server"; // is not secure to tell a client the server software and/or version
  this->_headers["Date"] = getDate();
  for ( std::map<std::string, std::string>::const_iterator it = this->_headers.begin(); it != this->_headers.end(); ++it ) {
    this->_response += it->first + ": " + it->second + kEOL;
  }

  this->_response += kEOL;

  if ( false == this->_body.empty() ) {
    this->_response += this->_body;
    this->_body.clear();
  }

  return ;
}

int Response::methodGet( void ) {
  if ( true == this->_requestData.getAutoIndex() && true == this->_responseData.isDirectory() ) {
    this->_body = this->_responseData.getAutoIndex( this->_requestData.getRequestRequestUri() );
    this->_headers["Content-Length"] = SSTR( this->_body.length() );
    this->_headers["Content-Type"] = this->kMimeTypes[".html"];
  } else {
    this->_body = this->_responseData.getFileContent();
    this->_headers["Content-Length"] = SSTR( this->_body.length() );
    this->_headers["Content-Type"] = this->kMimeTypes[this->_responseData.getExtension()];
  }
  return ( 200 ); // 200 ok
}

int Response::methodPost( void ) {
  const std::string & data = this->_requestData.getBody();

  if ( true == this->_responseData.fileExists() ) {
    this->_responseData.appendFile( data );
    this->_headers["Content-Length"] = SSTR( this->_body.length() );
    return ( 200 ); // 200 ok
  } else {
    this->_responseData.createFile( data );
    this->_headers["Content-Length"] = SSTR( this->_body.length() );
    this->_headers["Location"] = this->_requestData.getRequestUri();
    return ( 201 ); // 201 created
  }
}

int Response::methodPut( void ) {
  const std::string & data = this->_requestData.getBody();

  if ( true == this->_responseData.fileExists() ) {
    this->_responseData.createFile( data );
    this->_headers["Content-Length"] = SSTR( this->_body.length() );
    return ( 200 ); // 200 ok
  } else {
    this->_responseData.createFile( data );
    this->_headers["Content-Length"] = SSTR( this->_body.length() );
    this->_headers["Location"] = this->_requestData.getRequestUri();
    return ( 201 ); // 201 created
  }

  return ( 200 );
}

int Response::methodDelete( void ) {
  if ( false == this->_responseData.fileExists() ) {
    return ( 404 ); // 404 not found
  }

  this->_responseData.deleteFile();

  this->_body = "<html><body><h1>File deleted.</h1></body></html>";
  this->_headers["Content-Length"] = SSTR( this->_body.length() );
  this->_headers["Content-Type"] = this->kMimeTypes[".html"];

  return ( 200 ); // 200 ok
}

void Response::generateErrorPage( const int statusCode ) {
  // check if theres a page for the status code
  std::map<int, std::string> errorPages = this->_requestData.getErrorPages();
  if ( errorPages.end() != errorPages.find( statusCode ) ) {
    // redirect to error page
    this->_requestData.setMethod( "GET" );
    this->_redirect = true;
    this->_redirect_uri = errorPages[statusCode];
    this->_redirect_status_code = statusCode;
    this->_statusCode = 0;
  } else {
    // generate the ultimate error page
    this->_body = "<html><head><title>";
    this->_body += SSTR( statusCode );
    this->_body += "</title></head><body><h1>";
    this->_body += SSTR( statusCode );
    this->_body += "</h1><p>";
    if ( Response::kStatusCodes.end() != Response::kStatusCodes.find( statusCode ) ) {
      this->_body += Response::kStatusCodes[ statusCode ];
    } else {
      this->_body += "Error";
    }
    this->_body += "</p></body></html>";
    // set headers
    this->_headers["Content-Length"] = SSTR( this->_body.length() );
    this->_headers["Content-Type"] = this->kMimeTypes[".html"];
  }
  return ;
}

void Response::generateReturnPage( void ) {
  const std::pair<int, std::string> & returnPage = this->_requestData.getReturn();
  // set status code
  this->_statusCode = returnPage.first;
  // generate the ultimate redirection page
  this->_body = "<html><head><title>";
  this->_body += SSTR( returnPage.first );
  this->_body += "</title></head><body><h1>";
  this->_body += SSTR( returnPage.first );
  this->_body += "</h1><p>";
  // if return code is not a redirection ( 3XX ) different behaviour
  if ( returnPage.first < 300 || returnPage.first >= 400 ) {
    if ( true == returnPage.second.empty() ) {
      this->_body += Response::kStatusCodes[ returnPage.first ];
    } else {
      this->_body += returnPage.second;
    }
  } else {
    if ( Response::kStatusCodes.end() != Response::kStatusCodes.find( returnPage.first ) ) {
      this->_body += Response::kStatusCodes[ returnPage.first ];
    } else {
      this->_body += "Redirect";
    }
    // location header
    this->_headers["Location"] = returnPage.second;
  }
  this->_body += "</p></body></html>";
  // set headers
  this->_headers["Content-Length"] = SSTR( this->_body.length() );
  this->_headers["Content-Type"] = this->kMimeTypes[".html"];

  return ;
}

void Response::generateRedirectPage( const int statusCode, const std::string & uri ) {
  // set status code
  this->_statusCode = statusCode;
  // generate the ultimate redirection page
  this->_body = "<html><head><title>";
  this->_body += SSTR( statusCode );;
  this->_body += "</title></head><body><h1>";
  this->_body += SSTR( statusCode );
  this->_body += "</h1><p>";
  if ( Response::kStatusCodes.end() != Response::kStatusCodes.find( statusCode ) ) {
    this->_body += Response::kStatusCodes[ statusCode ];
  } else {
    this->_body += "Redirect";
  }
  // location header
  this->_headers["Location"] = uri;
  this->_body += "</p></body></html>";
  // set headers
  this->_headers["Content-Length"] = SSTR( this->_body.length() );
  this->_headers["Content-Type"] = this->kMimeTypes[".html"];

  return ;
}

} /** namespace webserv */
