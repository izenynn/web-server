#ifndef WEBSERV_SERVERCONFIG_H_
# define WEBSERV_SERVERCONFIG_H_

/** INCLUDES ----------------------------------- */

# include <string>
# include <vector>
# include <map>
# include <algorithm> // std::count()
# include <cstdio> // sscanf

/** CLASS -------------------------------------- */

namespace webserv {

struct Listen;

class ServerConfig {
  private:
    typedef std::vector<std::string> token_type;
  public:
    ServerConfig( void );
    ~ServerConfig( void );

    void print( const std::string & indent ) const;
    void parser( token_type::const_iterator & it );

    std::vector<Listen *> &         getListen( void );
    std::vector<std::string> &        getServerName( void );
    std::map<std::string, ServerConfig *> & getLocation( void );

    friend class RequestData;
  private:
    ServerConfig( const ServerConfig & other ); // not necessary
    ServerConfig & operator=( const ServerConfig & other ); // not necessary

    ServerConfig *  createLocationServerConfig( void );

    typedef void ( ServerConfig::*parse_directive_type )( token_type::const_iterator & );
    std::map<std::string, parse_directive_type> _serverDirectives;
    std::map<std::string, parse_directive_type> _locationDirectives;

    void parseLocation(       token_type::const_iterator & it );
    void parseListen(       token_type::const_iterator & it );
    void parseServerName(     token_type::const_iterator & it );
    void parseRoot(         token_type::const_iterator & it );
    void parseIndex(        token_type::const_iterator & it );
    void parseAutoindex(      token_type::const_iterator & it );
    void parseErrorPage(      token_type::const_iterator & it );
    void parseLimitExcept(      token_type::const_iterator & it );
    void parseClientMaxBodySize(  token_type::const_iterator & it );
    void parseUploadStore(      token_type::const_iterator & it );
    void parseAlias(        token_type::const_iterator & it );
    void parseReturn(       token_type::const_iterator & it );
    void parseCgi(          token_type::const_iterator & it );

    std::vector<Listen *>         _listen;
    std::map<std::string, ServerConfig *> _location;

    std::vector<std::string>      _server_name;
    std::string             _root;
    std::string             _alias;
    std::vector<std::string>      _index;
    bool                _autoindex;
    std::map<int, std::string>      _error_page;
    std::vector<std::string>      _limit_except;
    std::string::size_type        _client_max_body_size;
    std::string             _upload_store;
    std::pair<int, std::string>     _return;
    std::map<std::string, std::string>  _cgi;
  public:
    class ServerConfigException : virtual public std::exception {
      private:
        std::string message;
      public:
        ServerConfigException( const std::string & msg );
        virtual ~ServerConfigException( void ) throw ();
        virtual const char * what( void ) const throw ();
    };
};

} // namespace webserv

#endif // WEBSERV_SERVERCONFIG_H_
