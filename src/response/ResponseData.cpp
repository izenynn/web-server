/** INCLUDES ----------------------------------- */

#include <response/ResponseData.hpp>
#include <config/constants.hpp>
#include <types/nullptr_t.hpp>
#include <utils/utils.hpp>
#include <utils/log.hpp>

/** UTILS -------------------------------------- */

/** CLASS -------------------------------------- */

namespace webserv {

ResponseData::ResponseData()
    : _fd( 0 ) {
  return ;
}

ResponseData::~ResponseData() {
  this->closeFile();
  return ;
}

void ResponseData::print( void ) const {
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

void ResponseData::setPath( const std::string & path ) {
  this->_path = utils::sanitizePath( path );

  this->parseFileName();

  return ;
}

bool ResponseData::openFile( void ) {
  // close any open file
  this->closeFile();

  this->_fd = open( this->_path.c_str(), O_RDONLY );

  if ( this->_fd <= 0 ) {
    return ( false );
  } else {
    return ( true );
  }
}

void ResponseData::createFile( const std::string & data ) {
  this->closeFile();

  this->_fd = open( this->_path.c_str(), O_RDWR | O_CREAT | O_TRUNC, 00644 );
  if ( this->_fd <= 0 ) {
    LOG_FAILURE( "open() failed with return code -1 when opening: " << this->_path );
    return ;
  }

  if ( 0 == data.length() ) {
    return ;
  }

  ssize_t ret = write( this->_fd, data.c_str(), data.length() );
  if ( -1 == ret ) {
    LOG_FAILURE( "write() failed with return code -1 when writing to: " << this->_path );
  }

  return ;
}

void ResponseData::appendFile( const std::string & data ) {
  this->closeFile();

  this->_fd = open( this->_path.c_str(), O_RDWR | O_APPEND );
  if ( this->_fd <= 0 ) {
    LOG_FAILURE( "open() failed with return code -1 when opening: " << this->_path );
    return ;
  }

  if ( 0 == data.length() ) {
    return ;
  }

  ssize_t ret = write( this->_fd, data.c_str(), data.length() );
  if ( -1 == ret ) {
    LOG_FAILURE( "write() failed with return code -1 when writing to: " << this->_path );
  }

  return ;
}

void ResponseData::closeFile( void ) {
  if ( this->_fd <= 0 ) {
    return ;
  }

  close( this->_fd );
  this->_fd = 0;

  return ;
}

void ResponseData::deleteFile( void ) {
  if ( false == this->fileExists() ) {
    return ;
  }

  int ret = unlink( this->_path.c_str() );
  if ( -1 == ret ) {
    LOG_FAILURE( "unlink() failed with return code -1 when unlinking: " << this->_path );
  }

  return ;
}

const std::string ResponseData::getIndex( const std::vector<std::string> & indexes ) {
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
    closedir( d );
  } else {
    LOG_FAILURE( "opendir() failed with return code -1" );
  }

  ret = "";
  return ( ret );
}

bool ResponseData::isDirectory( void ) const {
  struct stat statbuf;
  stat( this->_path.c_str(), &statbuf );
  return ( S_ISDIR( statbuf.st_mode ) );
}

bool ResponseData::fileExists( void ) const {
  struct stat statbuf;
  int ret = stat( this->_path.c_str(), &statbuf );
  if ( ret == 0 ) {
    return ( true );
  } else {
    return ( false );
  }
}

std::string ResponseData::getAutoIndex( const std::string & uri ) {
  std::string body;
  DIR * d;
  struct dirent * e;
  struct stat statbuf;

  body = "<html><head><title>index of " + uri + "</title></head>";
  body += "<body><h1>index of " + uri + "</h1><hr>";

  d = opendir( this->_path.c_str() );
  if ( NULL != d ) {
    std::map<std::string, std::string> files; // <file name, html tag>
    std::string tag;
    // dir content
    while ( true ) {
      e = readdir( d );
      if ( NULL == e ) {
        break ;
      }

      std::string file = this->_path + "/" + e->d_name;
      stat( file.c_str(), &statbuf );

      tag = "<p><a href=\"" + utils::sanitizePath( uri + "/" + e->d_name ) + std::string( S_ISDIR( statbuf.st_mode ) ? "/" : "" ) + "\">";
      tag += e->d_name + std::string( S_ISDIR( statbuf.st_mode ) ? "/" : "" );
      tag += "</a></p>";

      files[e->d_name] = tag;
    }
    for ( std::map<std::string, std::string>::const_iterator it = files.begin(); it != files.end(); ++it ) {
      body += it->second;
    }
    closedir( d );
  } else {
    LOG_FAILURE( "opendir() failed with return code -1" );
    body += "error: could not open: " + uri;
  }

  body += "<hr></body></html>";

  return ( body );
}

const std::string & ResponseData::getExtension( void ) const {
  return ( this->_fileExtension );
}

const std::string ResponseData::getFileContent( void ) const {
  std::string content;
  char * buffer = new char[ (kReadBuffer + 1 ) * sizeof( char )];
  if ( NULL == buffer ) {
    LOG_FAILURE( "operator new char[] failed" );
    content = "";
    return ( content );
  }

  lseek( this->_fd, 0, SEEK_SET );
  while ( true ) {
    ssize_t ret = read( this->_fd, buffer, kReadBuffer );
    if ( 0 == ret ) {
      break ;
    }
    if ( -1 == ret ) {
      LOG_FAILURE( "read() failed with return code -1" );
      delete[] buffer;
      content = "";
      return ( content );
    }
    buffer[ret] = '\0';
    content.insert( content.length(), buffer, ret );
  }

  delete[] buffer;

  return ( content );
}

const std::string & ResponseData::getPath( void ) const {
  return ( this->_path );
}

void ResponseData::parseFileName( void ) {
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
