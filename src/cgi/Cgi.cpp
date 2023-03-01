/** INCLUDES ----------------------------------- */

# include <cgi/Cgi.hpp>
# include <config/constants.hpp>
# include <utils/utils.hpp>
# include <utils/log.hpp>

/** UTILS -------------------------------------- */

namespace {

// trim from left
inline std::string & ltrim( std::string & s, const char * t = " \t\n\r\f\v" ) {
  std::string::size_type index = s.find_first_not_of( t );
  if ( std::string::npos == index ) {
    return ( s );
  }
  s.erase( 0, index );
  return ( s );
}

// trim from right
inline std::string & rtrim( std::string & s, const char * t = " \t\n\r\f\v" ) {
  std::string::size_type index = s.find_last_not_of( t );
  if ( std::string::npos == index ) {
    return ( s );
  }
  s.erase( index + 1 );
  return ( s );
}

// trim from left & right
inline std::string & trim( std::string & s, const char * t = " \t\n\r\f\v" ) {
  return ( ltrim( rtrim( s, t ), t ) );
}

} /** namespace */

/** METHODS ------------------------------------ */

namespace webserv {

Cgi::Cgi( const RequestData & requestData, const ResponseData & responseData)
    : _requestData( requestData ),
      _responseData( responseData ) {
  this->_reqBody = this->_requestData.getBody();

  char * tmp = getcwd( NULL, 0 );
  if ( NULL == tmp ) {
    return;
  }
  std::string cwd = tmp;
  free ( tmp );

  this->_cgiBin = this->_requestData.getCgi().find( this->_responseData.getExtension() )->second;
  if ( '/' == this->_cgiBin[0] ) {
    this->_cgiPath = this->_cgiBin;
  } else {
    this->_cgiPath = cwd + "/" + this->_cgiBin;
  }

  this->_cgiTmpFilePath = kCgiTmpFile;
  this->_cgiTmpFileFd = open( this->_cgiTmpFilePath.c_str(), O_RDWR | O_CREAT | O_TRUNC, 00644 );
  if ( this->_cgiTmpFileFd <= 0 ) {
    return ;
  }

  this->_reqFilePath = cwd + "/" + this->_responseData.getPath();

  return ;
}

Cgi::~Cgi( void ) {
  if ( this->_cgiTmpFileFd > 0 ) {
    close( this->_cgiTmpFileFd );
    int ret = unlink( this->_cgiTmpFilePath.c_str() );
    if ( -1 == ret ) {
      LOG_FAILURE( "unlink() failed with return code -1 when unlinking: " + this->_cgiTmpFilePath );
    }
  }

  for ( size_t i = 0; NULL != this->_argv[i]; ++i ) {
    free( this->_argv[i] );
  }
  free( this->_argv );

  for ( size_t i = 0; NULL != this->_env[i]; ++i ) {
    free( this->_env[i] );
  }
  free( this->_env);

  return ;
}

int Cgi::exec( void ) {
  // env
  int ret = this->setEnv();
  if ( 0 != ret ) {
    return ( 500 ); // 500 internal server error
  }

  // argv
  this->_argv = reinterpret_cast<char **>( malloc( 3 * sizeof( char * ) ) );
  this->_argv[0] = strdup( this->_cgiPath.c_str() );
  this->_argv[1] = strdup( this->_reqFilePath.c_str() );
  this->_argv[2] = NULL;
  if ( NULL == this->_argv[0] || NULL == this->_argv[1] ) {
    return ( 500 ); // 500 internal server error
  }

  // execute
  int fd[2];
  ret = pipe(fd);
  if ( 0 != ret ) {
    return ( 500 ); // 500 internal server error
  }

  pid_t pid = fork();
  if ( -1 == pid ) {
    return ( 500 ); // 500 internal server error
  } else if ( pid > 0 ) {
    close( fd[READ_END] );
    ssize_t aux_ret = write( fd[WRITE_END], this->_reqBody.c_str(), this->_reqBody.length() );
    if ( -1 == aux_ret ) {
      return ( 500 ); // 500 internal server error
    }
    close( fd[WRITE_END] );

    int status;
    ret = waitpid( pid, &status, 0 );
    if ( -1 == ret || ( WIFEXITED(status) && 0 != WEXITSTATUS(status) ) ) {
      return ( 502 ); // 502 bad gateway
    }
  } else {
    if ( '/' == this->_reqFilePath[this->_reqFilePath.length() - 1] ) {
      this->_reqFilePath.erase( this->_reqFilePath.length() - 1 );
    }
    ret = chdir( this->_reqFilePath.substr( 0, this->_reqFilePath.find_last_of( '/' ) ).c_str() );
    if ( -1 == ret ) {
      return ( 500 ); // 500 internal server error
    }

    close( fd[WRITE_END] );
    ret = dup2( fd[READ_END], STDIN_FILENO );
    if ( -1 == ret ) {
      return ( 500 ); // 500 internal server error
    }
    close( fd[READ_END] );
    ret = dup2( this->_cgiTmpFileFd, STDOUT_FILENO );
    if ( -1 == ret ) {
      return ( 500 ); // 500 internal server error
    }

    execve( this->_argv[0], this->_argv, this->_env );

    LOG_ERROR( "execve() failed when running: " << this->_argv[0] );
    exit( 1 ); // error exit
  }

  // read cgi output and save into body
  char * buffer = new char[ (kReadBuffer + 1 ) * sizeof( char )];
  lseek( this->_cgiTmpFileFd, 0, SEEK_SET );
  for ( ssize_t aux_ret = 0; ; ) {
    aux_ret = read( this->_cgiTmpFileFd, buffer, kReadBuffer );
    if ( 0 == aux_ret ) {
      break ;
    }
    if ( -1 == aux_ret ) {
      LOG_FAILURE( "read() failed with return code -1" );
      //free( buffer );
      delete[] buffer;
      this->_body = "";
      return ( 500 ); // 500 internal server error
    }
    buffer[aux_ret] = '\0';
    this->_body.insert( this->_body.length(), buffer, aux_ret );
  }
  //free ( buffer );
  delete[] buffer;

  return ( 200 ); // 200 ok
}

void Cgi::getHeadersAndBody( std::map<std::string, std::string> & headers, std::string & body ) {
  std::string key, value;
  std::string::size_type sep;

  // iterate lines until body
  for ( std::string::size_type eol = this->_body.find( kEOL ); eol != std::string::npos; eol = this->_body.find( kEOL ) ) {
    // if no more headers (two consecutive new lines)
    if ( 0 == eol ) {
      this->_body.erase( 0, eol + kEOL.length() );
      break ;
    }

    // parse headers
    sep = this->_body.find( ':' );
    if ( std::string::npos == sep || 0 == sep ) {
      this->_body.erase( 0, eol + kEOL.length() );
      continue ;
    }
    key   = this->_body.substr( 0, sep );
    value = this->_body.substr( sep + 1, eol - sep - 1 );
    if ( headers.end() != headers.find( key ) ) {
      this->_body.erase( 0, eol + kEOL.length() );
      continue ;
    }
    headers[key] = trim( value, " " );
    if ( true == headers[key].empty() ) {
      headers.erase( key );
    }

    this->_body.erase( 0, eol + kEOL.length() );
  }

  // erase extra body if any
  std::map<std::string, std::string>::const_iterator contentLength = headers.find( "Content-Length" );
  if ( headers.end() != contentLength ) {
    this->_body.erase( atoi( contentLength->second.c_str() ) );
  }

  // body
  body = this->_body;

  return ;
}

int Cgi::setEnv( void ) {
  std::map<std::string, std::string> env;

  env["REDIRECT_STATUS"] = "200"; // security reasons, to tell the cgi the server handled the request

  env["GATEWAY_INTERFACE"]  = "CGI/1.1";
  env["SCRIPT_NAME"]      = this->_cgiPath;
  env["REQUEST_METHOD"]   = this->_requestData.getMethod();
  env["REQUEST_URI"]      = this->_reqFilePath;
  env["PATH_INFO"]      = this->_reqFilePath;
  env["PATH_TRANSLATED"]    = this->_reqFilePath;
  env["QUERY_STRING"]     = this->_requestData.getRequestQuery();
  env["REMOTE_ADDR"]      = this->_requestData.getHost();
  env["SERVER_NAME"]      = this->_requestData.getHost();
  env["SERVER_PORT"]      = SSTR( this->_requestData.getPort() );
  env["SERVER_PROTOCOL"]    = this->_requestData.getVersion();
  env["SERVER_SOFTWARE"]    = ""; // empty for security reasons

  if ( "POST" == this->_requestData.getMethod() || "PUT" == this->_requestData.getMethod() ) {
    env["CONTENT_LENGTH"] = SSTR( this->_reqBody.length() );
    env["CONTENT_TYPE"] = this->_requestData.getHeaders().find("Content-Type")->second;
  }

  for ( std::map<std::string, std::string>::const_iterator it = this->_requestData.getHeaders().begin(); it != this->_requestData.getHeaders().end(); ++it ) {
    if ( false == it->second.empty() ) {
      std::string header = it->first;
      std::transform( it->first.begin(), it->first.end(), header.begin(), static_cast<int(*)(int)>( std::toupper ) );
      std::replace( header.begin(), header.end(), '-', '_' );
      header.insert( 0, "HTTP_" );
      env[header] = it->second;
    }
  }

  // create **env
  this->_env = reinterpret_cast<char **>( malloc( ( env.size() + 1 ) * sizeof( char * ) ) );
  if ( webserv::nullptr_t == this->_env ) {
    return ( -1 );
  }

  char ** aux = this->_env;
  for ( std::map<std::string, std::string>::const_iterator it = env.begin(); it != env.end(); ++it ) {
    std::string var = it->first + "=" + it->second;
    *aux = strdup( var.c_str() );
    if ( NULL == *aux ) {
      return ( -1 );
    }
    ++aux;
  }
  *aux = NULL;

  return ( 0 );
}

} /** namespace webserv */
