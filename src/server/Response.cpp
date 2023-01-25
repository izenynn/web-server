/** INCLUDES ----------------------------------- */

#include <server/Response.hpp>
#include <utils/log.hpp>
#include <utils/utils.hpp>

/** UTILS -------------------------------------- */

namespace {

std::string getDate( void ) {
	struct timeval time;
	char buffer[128];

	gettimeofday( &time, NULL );

	struct tm *htime;
	htime = gmtime( &time.tv_sec );

	// Example: Date: Tue, 24 Dec 2022 19:22:21 GMT
	//int length = strftime( buffer, 32, "%a, %d %b %Y %T GMT", htime );
	int length = strftime( buffer, 32, "%a, %d %b %Y %H:%M:%S GMT", htime );

	return ( std::string( buffer, length ) );
}

}

/** CLASS -------------------------------------- */

namespace webserv {

std::map<int, std::string> Response::initStatusCodes() {
	std::map<int, std::string> status_codes;

	status_codes[100] = "Continue";
	status_codes[101] = "Switching Protocols";
	status_codes[102] = "Procossing";
	status_codes[103] = "Early Hints";

	status_codes[200] = "OK";
	status_codes[201] = "Created";
	status_codes[202] = "Accepted";
	status_codes[203] = "Non-Authoritative Information";
	status_codes[204] = "No Content";
	status_codes[205] = "Reset Content";
	status_codes[206] = "Partial Content";
	status_codes[207] = "Multi-Status";
	status_codes[208] = "Already Reported";
	status_codes[226] = "IM Used";

	status_codes[300] = "Multiple Choices";
	status_codes[301] = "Moved Permanently";
	status_codes[302] = "Found";
	status_codes[303] = "See Other";
	status_codes[304] = "Not Modified";
	status_codes[305] = "Use Proxy";
	status_codes[306] = "unused";
	status_codes[307] = "Temporary Redirect";
	status_codes[308] = "Permanent Redirect";

	status_codes[400] = "Bad Request";
	status_codes[401] = "Unauthorized";
	status_codes[402] = "Payment Required";
	status_codes[403] = "Forbidden";
	status_codes[404] = "Not Found";
	status_codes[405] = "Method Not Allowed";
	status_codes[406] = "Not Acceptable";
	status_codes[407] = "Proxy Authentication Required";
	status_codes[408] = "Request Timeout";
	status_codes[409] = "Conflict";
	status_codes[410] = "Gone";
	status_codes[411] = "Length Required";
	status_codes[412] = "Precondition Failed";
	status_codes[413] = "Payload Too Large";
	status_codes[414] = "URI Too Long";
	status_codes[415] = "Unsupported Media Type";
	status_codes[416] = "Range Not Satisfiable";
	status_codes[417] = "Expectation Failed";
	status_codes[418] = "I'm a teapot";
	status_codes[421] = "Misdirected Request";
	status_codes[422] = "Unprocessable Entity";
	status_codes[423] = "Locked";
	status_codes[424] = "Failed Dependency";
	status_codes[425] = "Too Early";
	status_codes[426] = "Upgrade Required";
	status_codes[428] = "Precondition Required";
	status_codes[429] = "Too Many Requests";
	status_codes[431] = "Request Header Fields Too Large";
	status_codes[451] = "Unavailable For Legal Reasons";

	status_codes[500] = "Internal Server Error";
	status_codes[501] = "Not Implemented";
	status_codes[502] = "Bad Gateway";
	status_codes[503] = "Service Unavailable";
	status_codes[504] = "Gateway Timeout";
	status_codes[505] = "HTTP Version Not Supported";
	status_codes[506] = "Variant Also Negotiates";
	status_codes[507] = "Insufficient Staroge";
	status_codes[508] = "Loop Detected";
	status_codes[510] = "Not Extended";
	status_codes[511] = "Network Authentication Required";

	return ( status_codes );
}

std::map<std::string, std::string> Response::initMimeTypes( void ) {
	std::map<std::string, std::string> mime_types;

	mime_types[".aac"]		= "audio/aac";
	mime_types[".abw"]		= "application/x-abiword";
	mime_types[".arc"]		= "application/x-freearc";
	mime_types[".avif"]		= "image/avif";
	mime_types[".avi"]		= "video/x-msvideo";
	mime_types[".azw"]		= "application/vnd.amazon.ebook";
	mime_types[".bin"]		= "application/octet-stream";
	mime_types[".bmp"]		= "image/bmp";
	mime_types[".bz"]		= "application/x-bzip";
	mime_types[".bz2"]		= "application/x-bzip2";
	mime_types[".cda"]		= "application/x-cdf";
	mime_types[".csh"]		= "application/x-csh";
	mime_types[".css"]		= "text/css";
	mime_types[".csv"]		= "text/csv";
	mime_types[".doc"]		= "application/msword";
	mime_types[".docx"]		= "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
	mime_types[".eot"]		= "application/vnd.ms-fontobject";
	mime_types[".epub"]		= "application/epub+zip";
	mime_types[".gz"]		= "application/gzip";
	mime_types[".gif"]		= "image/gif";
	mime_types[".htm"]		= "text/html";
	mime_types[".html"]		= "text/html";
	mime_types[".ico"]		= "image/vnd.microsoft.icon";
	mime_types[".ics"]		= "text/calendar";
	mime_types[".jar"]		= "application/java-archive";
	mime_types[".jpeg"]		= "image/jpeg";
	mime_types[".jpg"]		= "image/jpeg";
	mime_types[".js"]		= "text/javascript";
	mime_types[".json"]		= "application/json";
	mime_types[".jsonld"]	= "application/ld+json";
	mime_types[".mid"]		= "audio/midi";
	mime_types[".midi"]		= "audio/midi";
	mime_types[".mjs"]		= "text/javascript";
	mime_types[".mp3"]		= "audio/mpeg";
	mime_types[".mp4"]		= "video/mp4";
	mime_types[".mpeg"]		= "video/mpeg";
	mime_types[".mpkg"]		= "application/vnd.apple.installer+xml";
	mime_types[".odp"]		= "application/vnd.oasis.opendocument.presentation";
	mime_types[".ods"]		= "application/vnd.oasis.opendocument.spreadsheet";
	mime_types[".odt"]		= "application/vnd.oasis.opendocument.text";
	mime_types[".oga"]		= "audio/ogg";
	mime_types[".ogv"]		= "video/ogg";
	mime_types[".ogx"]		= "application/ogg";
	mime_types[".opus"]		= "audio/opus";
	mime_types[".otf"]		= "font/otf";
	mime_types[".png"]		= "image/png";
	mime_types[".pdf"]		= "application/pdf";
	mime_types[".php"]		= "application/x-httpd-php";
	mime_types[".ppt"]		= "application/vnd.ms-powerpoint";
	mime_types[".pptx"]		= "application/vnd.openxmlformats-officedocument.presentationml.presentation";
	mime_types[".rar"]		= "application/vnd.rar";
	mime_types[".rtf"]		= "application/rtf";
	mime_types[".sh"]		= "application/x-sh";
	mime_types[".svg"]		= "image/svg+xml";
	mime_types[".tar"]		= "application/x-tar";
	mime_types[".tif"]		= "image/tiff";
	mime_types[".tiff"]		= "image/tiff";
	mime_types[".ts"]		= "video/mp2t";
	mime_types[".ttf"]		= "font/ttf";
	mime_types[".txt"]		= "text/plain";
	mime_types[".vsd"]		= "application/vnd.visio";
	mime_types[".wav"]		= "audio/wav";
	mime_types[".weba"]		= "audio/webm";
	mime_types[".webm"]		= "video/webm";
	mime_types[".webp"]		= "image/webp";
	mime_types[".woff"]		= "font/woff";
	mime_types[".woff2"]	= "font/woff2";
	mime_types[".xhtml"]	= "application/xhtml+xml";
	mime_types[".xls"]		= "application/vnd.ms-excel";
	mime_types[".xlsx"]		= "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
	mime_types[".xml"]		= "application/xml";
	mime_types[".xul"]		= "application/vnd.mozilla.xul+xml";
	mime_types[".zip"]		= "application/zip";
	mime_types[".3gp"]		= "video/3gpp";
	mime_types[".3g2"]		= "video/3gpp2";
	mime_types[".7z"]		= "application/x-7z-compressed";

	return ( mime_types );
}

std::map<int, std::string>			Response::kStatusCodes = initStatusCodes();
std::map<std::string, std::string>	Response::kMimeTypes = initMimeTypes();

Response::Response( RequestConfig & config, int statusCode )
		: _statusCode( statusCode ),
		  _requestConfig( config ) {
	this->_methods["GET"]		= &Response::methodGet;
	//this->_methods["POST"]		= &Response::methodPost;
	//this->_methods["PUT"]		= &Response::methodPut;
	//this->_methods["DELETE"]	= &Response::methodDelete;
	return ;
}

Response::~Response( void ) {
	return ;
}

void Response::build( void ) {
	std::string & method = this->_requestConfig.getMethod();

	this->_uri.setPath( this->_requestConfig.getRoot() + "/" + this->_requestConfig.getRequestUri() );

	// check for errors and process request if none
	if ( false == this->_requestConfig.isValidMethod( method ) ) {
		// set error code
		this->_statusCode = 405; // 405 method not allowed
		// set allow header
		std::string allowedMethods;
		for ( std::vector<std::string>::const_iterator it = this->_requestConfig.getAllowedMethods().begin(); it != this->_requestConfig.getAllowedMethods().end(); ) {
			allowedMethods += *it;
			++it;
			if ( this->_requestConfig.getAllowedMethods().end() != it ) {
				allowedMethods += ", ";
			}
		}
		this->_headers["Allow"] = allowedMethods;
	} else if ( this->_requestConfig.getBody().length() > this->_requestConfig.getMaxBodySize() ) {
		this->_statusCode = 413; // 413 payload too large
	} else {
		this->_statusCode = this->process(); // process method
	}

	// check if error page needed
	if ( this->_statusCode >= 300 && 0 == this->_body.length() ) {
		this->generateErrorPage( this->_statusCode );
	}

	// check if not redirection
	if ( false == this->_redirect ) {
		this->setResponse();
	}

	return ;
}

const std::string & Response::getResponse( void ) {
	return ( this->_response );
}

bool Response::getRedirect( void ) {
	return ( this->_redirect );
}

std::string Response::getRedirectUri( void ) {
	return ( this->_redirect_uri );
}

int Response::process( void ) {
	const std::string & method = this->_requestConfig.getMethod();

	// get (get cant be cig)
	if ( "GET" == method ) {
		// directory, if index -> go to index, else if no index and no autoindex -> bad request
		if ( true == this->_uri.isDirectory() ) {
			std::string index = this->_uri.getIndex( this->_requestConfig.getIndex() );
			if ( index.length() > 0 ) {
				this->_redirect = true;
				this->_redirect_uri = utils::sanitizePath( "/" + this->_requestConfig.getRequestUri() + "/" + index );
				return ( 200 ); // 200 ok
			} else if ( false == this->_requestConfig.getAutoIndex() ) {
				return ( 404 ); // 404 bad request
			}
		// not directory
		} else {
			if ( false == this->_uri.fileExists() ) {
				return ( 404 ); // 404 bad request
			}

			// TODO getFile()
			this->_uri.getFile();

			if ( this->_uri.openFile() ) {
				return ( 403 ); // 403 forbidden
			}
		}

	}

	// check cgi
	// TODO cgi

	// post / put
	if ( "POST" == method || "PUT" == method ) {
		//std::string path = this->_requestConfig.getLocationUri() + "/" + this->_requestConfig.getRequestUri();
		this->_statusCode = 405; // 405 method not allowed
	}

	int ret = (this->*(Response::_methods[method]))();
	return ( ret );
}

void Response::setResponse( void ) {
	if ( this->_statusCode < 400 && this->_redirect_status_code != 0 ) {
		this->_statusCode = this->_redirect_status_code;
	}

	std::string status = SSTR( this->_statusCode ) + " " + this->kStatusCodes[this->_statusCode];
	this->_response += this->_requestConfig.getVersion() + " " + status + Config::kEOL;

	this->_headers["Date"] = getDate();

	for ( std::map<std::string, std::string>::const_iterator it = this->_headers.begin(); it != this->_headers.end(); ++it ) {
		this->_response += it->first + ": " + it->second + Config::kEOL;
	}

	this->_response += Config::kEOL;

	if ( false == this->_body.empty() ) {
		this->_response += this->_body;
		this->_body.clear();
	}

	return ;
}

int Response::methodGet( void ) {
	// TODO autoindex
	if ( true == this->_requestConfig.getAutoIndex() && true == this->_uri.isDirectory() ) {
		this->_body = this->_uri.getAutoIndex( this->_requestConfig.getRequestRequestUri() );
		this->_headers["Content-Length"] = SSTR( this->_body.length() );
		this->_headers["Content-Type"] = this->kMimeTypes[".html"];
	} else {
		this->_body = this->_uri.getContent();
		this->_headers["Content-Length"] = SSTR( this->_body.length() );
		this->_headers["Content-Type"] = this->kMimeTypes[this->_uri.getExtension()];
	}
	return ( 200 ); // 200 ok
}

void Response::generateErrorPage( int statusCode ) {
	// check if theres a page for the status code
	std::map<int, std::string> errorPages = this->_requestConfig.getErrorPages();
	if ( errorPages.end() != errorPages.find( statusCode ) ) {
		// redirect to error page
		this->_requestConfig.getMethod() = "GET";
		this->_redirect = true;
		this->_redirect_uri = errorPages[statusCode];
		this->_redirect_status_code = statusCode;
		this->_statusCode = 0; // FIXME
	} else {
		// generate the ultimate error page
		this->_body = "<html><head><title>Error</title></head><body><h1>";
		this->_body += SSTR( statusCode );
		this->_body += "</h1><br><p>";
		if ( Response::kStatusCodes.end() != Response::kStatusCodes.find( statusCode ) ) {
			this->_body += Response::kStatusCodes[ statusCode ];
		} else {
			this->_body += "Error";
		}
		this->_body += "</p></body></html>";
	}
	return ;
}

} /** namespace webserv */
