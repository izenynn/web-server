#pragma once

#ifndef __CGI_HPP__
# define __CGI_HPP__

/** INCLUDES ----------------------------------- */

# include <map>
# include <string>

/** DEFINES ----------------------------------- */

# define READ_END 0
# define WRITE_END 1

/** CLASS -------------------------------------- */

namespace webserv {

class RequestData;
class ResponseData;

class Cgi {
  public:
    Cgi( const RequestData & requestData, const ResponseData & responseData );
    ~Cgi( void );

    int exec( void );

    void getHeadersAndBody( std::map<std::string, std::string> & headers, std::string & body );
  private:
    int setEnv( void );

    const RequestData &   _requestData;
    const ResponseData &  _responseData;

    std::string       _reqBody;
    std::string       _reqFilePath;

    std::string       _cgiBin;
    std::string       _cgiPath;

    std::string       _cgiTmpFilePath;
    int           _cgiTmpFileFd;

    char **         _argv;
    char **         _env;

    std::string       _body;
};

} /** namespace webserv */

#endif /** __CGI_HPP__ */
