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

void Uri::print( void ) const {
	std::string i = "        ";
	std::cout << "    " << "\nRESPONSE URI:" << std::endl;

	std::cout << i << "path:      " << this->_path << std::endl;
	std::cout << i << "fd:        " << this->_fd << std::endl;
	std::cout << i << "file name: " << this->_fileName << std::endl;
	std::cout << i << "file ext:  " << this->_fileExtension << std::endl;

	std::cout << i << "is dir:    " << std::boolalpha << this->isDirectory() << std::endl;
	std::cout << i << "exists:    " << std::boolalpha << this->fileExists() << std::endl;

	return ;
}

void Uri::setPath( const std::string & path ) {
	this->_path = utils::sanitizePath( path );

	this->parseFileName();

	return ;
}

bool Uri::openFile( void ) {
	log::warning("### opening file..."); // DEBUG
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

	log::warning("### closing file..."); // DEBUG
	close( this->_fd );
	this->_fd = 0;

	return ;
}

const std::string Uri::getIndex( std::vector<std::string> & indexes ) {
	std::string ret;
	DIR * d;
	struct dirent * e;

	d = opendir( this->_path.c_str() );
	if ( NULL != d ) {
		while ( true ) {
			e = readdir( d );
			if ( NULL == e ) {
				break ;
			}

			for ( std::vector<std::string>::const_iterator it = indexes.begin(); it != indexes.end(); ++it ) {
				if ( e->d_name == *it ) {
					ret = "/" + std::string( e->d_name );
					closedir( d );
					return ( ret );
				}
			}
		}
	} else {
		log::failure( "opendir() failed with return code -1" );
	}

	ret = "";
	return ( ret );
}

bool Uri::isDirectory( void ) const {
	struct stat statbuf;
	stat( this->_path.c_str(), &statbuf );
	return ( S_ISDIR( statbuf.st_mode ) );
}

bool Uri::fileExists( void ) const {
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
	if ( NULL == buffer ) {
		log::failure( "malloc() failed with return code -1" );
		content = "";
		return ( content );
	}

	lseek( this->_fd, 0, SEEK_SET );
	std::cout << "### fd: " << this->_fd << std::endl;
	while ( true ) {
		int ret = read( this->_fd, buffer, Uri::kReadBuffer );
		if ( 0 == ret ) {
			break ;
		}
		if ( -1 == ret ) {
			log::failure( "read() failed with return code -1" );
			free( buffer );
			content = "";
			return ( content );
		}
		buffer[ret] = '\0';
		content.insert( content.length(), buffer, ret );
		std::cout << "### readed: " << buffer << std::endl;
	}

	free ( buffer );
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
