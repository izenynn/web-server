#ifndef WEBSERV_CGI_H_
# define WEBSERV_CGI_H_

/** INCLUDES ----------------------------------- */

# include <map>
# include <string>

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
    static const int kReadEnd = 0;
    static const int kWriteEnd = 1;

    int setEnv( void );

    const RequestData &   _requestData;
    const ResponseData &  _responseData;

    std::string       _reqBody;
    std::string       _reqFilePath;

    std::string       _cgiBin;
    std::string       _cgiPath;

    std::string       _cgiTmpFilePath;
    int               _cgiTmpFileFd;

    char **           _argv;
    char **           _env;

    std::string       _body;
};

} // namespace webserv

#endif // WEBSERV_CGI_H_
