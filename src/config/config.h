#ifndef WEBSERV_CONFIG_H_
# define WEBSERV_CONFIG_H_

/** INCLUDES ----------------------------------- */

# include <string>
# include <vector>

/** CLASS -------------------------------------- */

namespace webserv {

class ServerConfig;
class Client;

class Config {
  private:
    typedef std::vector<std::string> token_type;
  public:
    Config( void );
    virtual ~Config( void );

    void print( void );

    void load( const char* file );

    const std::vector<ServerConfig *> * getServers( void ) const;
  private:
    Config( const Config & other ); // not necessary
    Config &operator=( const Config& other ); // not necessary

    const char *                  _file;
    std::vector<std::string>      _tokens;
    std::vector<ServerConfig *>   _server;

    void        lexer( void );
    void        parser( void );
  public:
    class ConfigException : virtual public std::exception {
      private:
        std::string message;
      public:
        ConfigException( const std::string & msg );
        virtual ~ConfigException( void ) throw ();
        virtual const char * what( void ) const throw ();
    };
};

} // namespace webserv

#endif // WEBSERV_CONFIG_H_
