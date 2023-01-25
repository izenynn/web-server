/** INCLUDES ----------------------------------- */

#include <server/Uri.hpp>
#include <utils/utils.hpp>
#include <types/nullptr_t.hpp>

/** UTILS -------------------------------------- */

/** CLASS -------------------------------------- */

namespace webserv {

Uri::Uri()
		: _fd( 0 ) {
	return ;
}

Uri::~Uri() {
	this->closeFile();
	return ;
}

void Uri::setPath( const std::string & path ) {
	this->_path = utils::sanitizePath( path );

	this->parseExtension();

	return ;
}

bool Uri::openFile( void ) {
	// close any open file
	this->closeFile();

	this->_fd = open( this->_path.c_str(), O_RDONLY );

	if ( this->_fd <= 0 ) {
		return ( false );
	} else {
		return ( true );
	}
}

void Uri::closeFile( void ) {
	if ( this->_fd <= 0 ) {
		return ;
	}

	close( this->_fd );
	this->_fd = 0;

	return ;
}

void getFile( void ) {
	;
}

bool Uri::isDirectory( void ) {
	struct stat statbuf;
	stat( this->_path.c_str(), &statbuf );
	return ( S_ISDIR( statbuf.st_mode ) );
}

bool Uri::fileExists( void ) {
	struct stat statbuf;
	int ret = stat( this->_path.c_str(), &statbuf );
	if ( ret == 0 ) {
		return ( true );
	} else {
		return ( false );
	}
}

std::string Uri::getAutoIndex( const std::string & uri ) {
	// TODO autoindex
	(void)uri;
	std::string body = "<html><head><title>TODO</title></head><body><h1>TODO</h1></body></html>";
	return ( body );
}

} /** namespace webserv */
