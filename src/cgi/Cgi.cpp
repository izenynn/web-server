/** INCLUDES ----------------------------------- */

# include <cgi/Cgi.hpp>
# include <utils/log.hpp>

/** METHODS ------------------------------------ */

namespace webserv {

Cgi::Cgi( const RequestConfig & requestConfig, const ResponseData & responseData)
		: _requestConfig( requestConfig ),
		  _responseData( responseData ) {
	/*if ( this->_requestConfig->getMethod() == "GET" ) {
		this->_requestBody = this->_responseData->getFileContent();
	} else {
		this->_requestBody = this->_requestConfig->getBody();
	}*/
	this->_requestBody = this->_requestConfig.getBody();

	//

	return ;
}

Cgi::~Cgi( void ) {
	return ;
}

int Cgi::exec( void ) {
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

} /** namespace webserv */
