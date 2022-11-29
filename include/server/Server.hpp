#ifndef __SERVER_HPP__
# define __SERVER_HPP__

/**	INCLUDES ----------------------------------- */

# include <string>
//# include <sys/socket.h>
//# include <netinet/in.h>

# include "nullptr_t.hpp"
# include "config/Config.hpp"

/*
===============================================================================
Server: A server listens on a connection for a request,
   parses each message received, interprets the message semantics in
   relation to the identified request target, and responds to that
   request with one or more response messages.
===============================================================================
*/

class Server {
	private:
		static const char* DEFAULT_PATH;
	public:
		Server( void );
		Server& operator=( const Server& other);
		~Server( void );

		void configLoad(); // default path
		void configLoad( const char* file );
		int run( void );

	protected:
	private:
		Config *_config;
	/*	int					socket;
		int					domain; //AF_INET (IP), AF_INET6 (IPv6), AF_UNSPEC, AF_UNIX, AF_ISO (ISO protocols), and AF_NS (Xerox Network Systems protocols).
		int					type; //SOCK_STREAM (virtual circuit service), SOCK_DGRAM (datagram service), SOCK_RAW (direct IP service)
		int					protocol; // The protocol should usually be set to zero to indicate that the default protocol should be used
		struct sockaddr_in	serverInfo;
		u_long				hostlong; // Should be set to 'AF_INET'
		int					backlog;	// Is the number of connections allowed on the incoming queue
		int					serverFD; // Socket
		


							apsServer();
							apsServer( const apsServer & data );
							apsServer &	operator=( const apsServer & data );
							~apsServer();

		int					GetDomain();
		int					GetType();
		int					GetProtocol();
		int					GetHostlong();
		int					GetBacklog();
		int					GetServerFD();*/
};

#endif // __SERVER_HPP__
