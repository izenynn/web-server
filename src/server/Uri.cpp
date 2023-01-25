/** INCLUDES ----------------------------------- */

#include <server/Uri.hpp>
#include <utils/utils.hpp>
#include <utils/log.hpp>
#include <types/nullptr_t.hpp>

/** UTILS -------------------------------------- */

/** CLASS -------------------------------------- */

namespace webserv {

const size_t Uri::kReadBuffer = 8192;

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

	this->parseFileName();

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

const std::string & Uri::getExtension( void ) {
	return ( this->_fileExtension );
}

const std::string Uri::getFileContent( void ) {
	std::string content;
	char * buffer = reinterpret_cast<char *>( malloc( ( Uri::kReadBuffer + 1 ) * sizeof( char ) ) );

	lseek( this->_fd, 0, SEEK_SET );
	while ( true ) {
		int ret = read( this->_fd, &buffer, Uri::kReadBuffer );
		if ( 0 == ret ) {
			break ;
		}
		if ( -1 == ret ) {
			log::failure( "read() failed with return code -1" );
			content = "";
			return ( content );
		}
		buffer[ret] = '\0';
		content.insert( content.length(), buffer, ret );
	}

	return ( content );
}

void Uri::parseFileName( void ) {
	std::string fileName = this->_path.substr( this->_path.find_last_of( "/" ) + 1 );
	if ( true == fileName.empty() ) {
		// is a directory
		return ;
	}

	// save file name
	this->_fileName = fileName;

	// save extension if any
	if ( std::string::npos != fileName.find_last_of( "." ) ) {
		this->_fileExtension = fileName.substr( fileName.find_last_of( "." ) );
	}
}

} /** namespace webserv */
