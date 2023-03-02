#ifndef WEBSERV_SERVER_H_
# define WEBSERV_SERVER_H_

/** INCLUDES ----------------------------------- */

# include <string> // std::string
# include <map>
# include <list>
# include <vector>

# include <config/config.h>
# include <server/client.h>

/** CLASS -------------------------------------- */

namespace webserv {

class Server {
  public:
    Server( void );
    ~Server( void );

    void load( void ); // default path
    void load( const char* file );
    void print( void );

    int start( void );
    static int stop( void );

  private:
    Server & operator=( const Server & other); // not necessary

    int clientRecv( int fd );
    int clientSend( int fd );

    void newClient( int fd );
    void disconnectClient( int fd );
    void checkDisconnectClient( Client * client );

    void disconnectServer( int fd );

    void addToFdSet( int fd );
    void delFromFdSet( int fd );

    static bool _run;

    Config *              _config;
    const std::vector<ServerConfig *> * _serverConfigs;

    std::map<int, Listen *>       _servers;
    std::map<int, Client *>       _clients;

    std::list<int>  _fdList;
    fd_set      _fdSet;
    fd_set      _fdRead;
    fd_set      _fdWrite;
    int       _fdMax;

    int initialize( void );
};

} // namespace webserv

#endif // WEBSERV_SERVER_H_
