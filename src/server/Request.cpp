/** INCLUDES ----------------------------------- */

#include <server/Request.hpp>

/** UTILS -------------------------------------- */

namespace {

bool isSupportedMethod( const std::string & method ) {
	bool ret = false;

	if ( "GET" == method ) ret = true;
	else if ( "POST" == method ) ret = true;
	else if ( "PUT" == method ) ret = true;
	else if ( "HEAD" == method ) ret = true;
	else if ( "DELETE" == method ) ret = true;

	return ( ret );
}

bool isSupportedVersion( const std::string & version ) {
	bool ret = false;

	if ( "HTTP/1.1" == version ) ret = true;

	return ( ret );
}

bool isForbiddenUri( const std::string & uri ) {
	std::string path = uri;
	int cnt = 0;

	while ( std::string::npos != path.find( '/' ) ) {
		// take all except first '/' on each iteration
		// /images/bg.png
		//  ~~~~~~~~~~~~~
		path = path.substr( path.find( '/' ) + 1 );
		if ( true == path.empty() ) {
			break ;
		}

		// take folder/file name (in case no '/' find returns npos, so it takes the rest of the string)
		// images/bg.png
		// ~~~~~~
		std::string file = path.substr( 0, path.find( '/' ) );
		if ( std::string::npos != file.find( "..", 0 ) ) {
			--cnt;
		} else {
			++cnt;
		}
	}

	if ( cnt < 0 ) {
		return ( true );
	}
	return ( false );
}

} /** namespace */

/** CLASS -------------------------------------- */

namespace webserv {

Request::Request( void ) {
	return ;
}

Request::~Request( void ) {
	return ;
}

int Request::parse( const std::string & buffer ) {
	int ret;

	this->_buffer += buffer;
	//buffer.clear();

	if ( this->kRequestLine == this->_status ) {
		ret = this->parseRequestLine();
	}
	if ( this->kHeaders == this->_status ) {
		ret = this->parseHeaders();
	}
	if ( this->kBody == this->_status ) {
		ret = this->parseBody();
	}
	if ( this->kChunk == this->_status ) {
		ret = this->parseChunk();
	}

	if ( this->kComplete == this->_status ) {
		return ( ret );
	}
	if ( this->kError == this->_status ) {
		return ( ret );
	}

	if ( ret > 0 ) {
		this->_status = this->kError;
		return ( ret );
	}

	return ( 1 ); // it will never get here
}

int Request::parseRequestLine( void ) {
	if ( std::string::npos != this->_buffer.find( "\r\n" ) ) {
		// method
		if ( std::string::npos == this->_buffer.find( ' ' ) || 0 == this->_buffer.find( ' ' ) ) {
			return ( 400 ); // 400 bad request
		}
		std::string token = this->_buffer.substr( 0, _buffer.find(' ') );
		if ( false == isSupportedMethod( token ) ) {
			this->_status = this->kError;
			return ( 501 ); // 501 not implemented
		}
		this->_method = token;
		this->_buffer.erase( 0, this->_method.length() + 1 );

		// uri
		if ( std::string::npos == this->_buffer.find( ' ' ) || 0 == this->_buffer.find( ' ' ) ) {
			return ( 400 ); // 400 bad request
		}
		token = this->_buffer.substr( 0, _buffer.find(' ') );
		if ( '/' != token[0] ) {
			return ( 400 ); // 400 bad request
		}
		if ( true == isForbiddenUri( token ) ) {
			return ( 403 ); // 403 forbidden
		}
		if ( token.length() >= Config::kLimitRequestLimit ) {
			return ( 414 ); // 414 uri too long
		}
		this->_request_uri = token;
		this->_buffer.erase( 0, this->_request_uri.length() + 1 );

		if ( std::string::npos != this->_request_uri.find( '?' ) ) {
			std::string::size_type index = this->_request_uri.find( '?' );
			this->_uri_params = this->_request_uri.substr( index + 1 );
			this->_request_uri.erase( index );
		}

		// version
		if ( 0 == this->_buffer.find( ' ' ) ) {
			return ( 400 ); // 400 bad request
		}
		std::string::size_type last = this->_buffer.find( "\r\n" );
		token = this->_buffer.substr( 0, last );

		if ( false == isSupportedVersion( token ) ) {
			return ( 505 ); // 505 http version not supported
		}
		this->_version = token;
		this->_buffer.erase( 0, last + 2 );

		// status
		this->_status = this->kHeaders;
	}
	return ( 0 );
}

} /** namespace webserv */
