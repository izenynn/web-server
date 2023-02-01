/** INCLUDES ----------------------------------- */

# include <cgi/Cgi.hpp>
# include <config/constants.hpp>
# include <utils/log.hpp>

/** METHODS ------------------------------------ */

namespace webserv {

Cgi::Cgi( const RequestConfig & requestConfig, const ResponseData & responseData)
		: _requestConfig( requestConfig ),
		  _responseData( responseData ) {
	// FIXME \/
	/*if ( this->_requestConfig->getMethod() == "GET" ) {
		this->_requestBody = this->_responseData->getFileContent();
	} else {
		this->_requestBody = this->_requestConfig->getBody();
	}*/
	this->_reqBody = this->_requestConfig.getBody();

	char * cwd = getcwd( NULL, 0 );
	if ( NULL == cwd ) {
		return;
	}

	this->_cgiBin = this->_requestConfig.getCgi().find( this->_responseData.getExtension() )->second;
	if ( '/' == this->_cgiBin[0] ) {
		this->_cgiPath = this->_cgiBin;
	} else {
		this->_cgiPath = std::string( cwd ) + "/" + this->_cgiBin;
	}

	this->_cgiTmpFilePath = kCgiTmpFile;
	this->_cgiTmpFileFd = open( this->_cgiTmpFilePath.c_str(), O_RDWR | O_CREAT | O_TRUNC, 00644 );
	if ( this->_cgiTmpFileFd <= 0 ) {
		return ;
	}

	this->_reqFilePath = std::string ( cwd ) + "/" + this->_responseData.getPath();

	return ;
}

Cgi::~Cgi( void ) {
	if ( this->_cgiTmpFileFd > 0 ) {
		close( this->_cgiTmpFileFd );
		int ret = unlink( this->_cgiTmpFilePath.c_str() );
		if ( -1 == ret ) {
			log::failure( "unlink() failed with return code -1 when unlinking: " + this->_cgiTmpFilePath );
		}
	}

	for ( size_t i = 0; NULL != this->_env[i]; ++i ) {
		free( this->_env[i] );
	}
	delete[] this->_env;

	return ;
}

int Cgi::exec( void ) {
	int ret = this->setEnv();
	if ( 0 != ret ) {
		return ( 500 );
	}

	// TODO

	return ( 500 );
}

void Cgi::getHeaders( std::map<std::string, std::string> & headers ) {
	(void)headers;
	return ;
}

void Cgi::getBody( std::string & body ) {
	(void)body;
	return ;
}

int Cgi::setEnv( void ) {
	std::map<std::string, std::string> env;

	env["REDIRECT_STATUS"] = "200"; // security reasons, to tell the cgi the server handled the request

	env["GATEWAY_INTERFACE"]	= "CGI/1.1";
	env["SCRIPT_NAME"]			= this->_cgiPath;
	env["REQUEST_METHOD"]		= this->_requestConfig.getMethod();
	env["REQUEST_URI"]			= this->_reqFilePath;
	env["PATH_INFO"]			= this->_reqFilePath;
	env["PATH_TRANSLATED"]		= this->_reqFilePath;
	env["QUERY_STRING"]			= this->_requestConfig.getRequestQuery();
	env["REMOTE_ADDR"]			= this->_requestConfig.getHost();
	env["SERVER_NAME"]			= this->_requestConfig.getHost();
	env["SERVER_PORT"]			= SSTR( this->_requestConfig.getPort() );
	env["SERVER_PROTOCOL"]		= this->_requestConfig.getVersion();
	env["SERVER_SOFTWARE"]		= ""; // empty for security reasons

	if ( "POST" == this->_requestConfig.getMethod() || "PUT" == this->_requestConfig.getMethod() ) {
		env["CONTENT_LENGTH"] = SSTR( this->_reqBody.length() );
		env["CONTENT_TYPE"] = this->_requestConfig.getHeaders().find("Content-Type")->second;
	}

	for ( std::map<std::string, std::string>::const_iterator it = this->_requestConfig.getHeaders().begin(); it != this->_requestConfig.getHeaders().end(); ++it ) {
		if ( false == it->second.empty() ) {
			std::string header = it->first;
			std::transform( it->first.begin(), it->first.end(), header.begin(), static_cast<int(*)(int)>( std::toupper ) );
			std::replace( header.begin(), header.end(), '-', '_' );
			header.insert( 0, "HTTP_" );
			env[header] = it->second;
		}
	}

	// create **env
	this->_env = new ( std::nothrow ) char*[ ( env.size() + 1 ) * sizeof( char * ) ];
	if ( webserv::nullptr_t == this->_env ) {
		return ( -1 );
	}

	log::debug( "HEADERS" );
	char ** aux = this->_env;
	for ( std::map<std::string, std::string>::const_iterator it = env.begin(); it != env.end(); ++it ) {
		std::string var = it->first + "=" + it->second;
		*aux = strdup( var.c_str() );
		if ( NULL == *aux ) {
			return ( -1 );
		}
		log::debug( "> " + var );
		++aux;
	}
	*aux = NULL;

	return ( 0 );
}

} /** namespace webserv */
