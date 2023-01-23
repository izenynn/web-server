/** INCLUDES ----------------------------------- */

#include <server/Response.hpp>

/** UTILS -------------------------------------- */

/** CLASS -------------------------------------- */

namespace webserv {

Response::Response( void ) {
	this->_status_codes[100] = "Continue";
	this->_status_codes[101] = "Switching Protocols";
	this->_status_codes[102] = "Procossing";
	this->_status_codes[103] = "Early Hints";

	this->_status_codes[200] = "OK";
	this->_status_codes[201] = "Created";
	this->_status_codes[202] = "Accepted";
	this->_status_codes[203] = "Non-Authoritative Information";
	this->_status_codes[204] = "No Content";
	this->_status_codes[205] = "Reset Content";
	this->_status_codes[206] = "Partial Content";
	this->_status_codes[207] = "Multi-Status";
	this->_status_codes[208] = "Already Reported";
	this->_status_codes[226] = "IM Used";

	this->_status_codes[300] = "Multiple Choices";
	this->_status_codes[301] = "Moved Permanently";
	this->_status_codes[302] = "Found";
	this->_status_codes[303] = "See Other";
	this->_status_codes[304] = "Not Modified";
	this->_status_codes[305] = "Use Proxy";
	this->_status_codes[306] = "unused";
	this->_status_codes[307] = "Temporary Redirect";
	this->_status_codes[308] = "Permanent Redirect";

	this->_status_codes[400] = "Bad Request";
	this->_status_codes[401] = "Unauthorized";
	this->_status_codes[402] = "Payment Required";
	this->_status_codes[403] = "Forbidden";
	this->_status_codes[404] = "Not Found";
	this->_status_codes[405] = "Method Not Allowed";
	this->_status_codes[406] = "Not Acceptable";
	this->_status_codes[407] = "Proxy Authentication Required";
	this->_status_codes[408] = "Request Timeout";
	this->_status_codes[409] = "Conflict";
	this->_status_codes[410] = "Gone";
	this->_status_codes[411] = "Length Required";
	this->_status_codes[412] = "Precondition Failed";
	this->_status_codes[413] = "Payload Too Large";
	this->_status_codes[414] = "URI Too Long";
	this->_status_codes[415] = "Unsupported Media Type";
	this->_status_codes[416] = "Range Not Satisfiable";
	this->_status_codes[417] = "Expectation Failed";
	this->_status_codes[418] = "I'm a teapot";
	this->_status_codes[421] = "Misdirected Request";
	this->_status_codes[422] = "Unprocessable Entity";
	this->_status_codes[423] = "Locked";
	this->_status_codes[424] = "Failed Dependency";
	this->_status_codes[425] = "Too Early";
	this->_status_codes[426] = "Upgrade Required";
	this->_status_codes[428] = "Precondition Required";
	this->_status_codes[429] = "Too Many Requests";
	this->_status_codes[431] = "Request Header Fields Too Large";
	this->_status_codes[451] = "Unavailable For Legal Reasons";

	this->_status_codes[500] = "Internal Server Error";
	this->_status_codes[501] = "Not Implemented";
	this->_status_codes[502] = "Bad Gateway";
	this->_status_codes[503] = "Service Unavailable";
	this->_status_codes[504] = "Gateway Timeout";
	this->_status_codes[505] = "HTTP Version Not Supported";
	this->_status_codes[506] = "Variant Also Negotiates";
	this->_status_codes[507] = "Insufficient Staroge";
	this->_status_codes[508] = "Loop Detected";
	this->_status_codes[510] = "Not Extended";
	this->_status_codes[511] = "Network Authentication Required";

	this->_mime_types[".aac"]		= "audio/aac";
	this->_mime_types[".abw"]		= "application/x-abiword";
	this->_mime_types[".arc"]		= "application/x-freearc";
	this->_mime_types[".avif"]		= "image/avif";
	this->_mime_types[".avi"]		= "video/x-msvideo";
	this->_mime_types[".azw"]		= "application/vnd.amazon.ebook";
	this->_mime_types[".bin"]		= "application/octet-stream";
	this->_mime_types[".bmp"]		= "image/bmp";
	this->_mime_types[".bz"]		= "application/x-bzip";
	this->_mime_types[".bz2"]		= "application/x-bzip2";
	this->_mime_types[".cda"]		= "application/x-cdf";
	this->_mime_types[".csh"]		= "application/x-csh";
	this->_mime_types[".css"]		= "text/css";
	this->_mime_types[".csv"]		= "text/csv";
	this->_mime_types[".doc"]		= "application/msword";
	this->_mime_types[".docx"]		= "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
	this->_mime_types[".eot"]		= "application/vnd.ms-fontobject";
	this->_mime_types[".epub"]		= "application/epub+zip";
	this->_mime_types[".gz"]		= "application/gzip";
	this->_mime_types[".gif"]		= "image/gif";
	this->_mime_types[".htm"]		= "text/html";
	this->_mime_types[".html"]		= "text/html";
	this->_mime_types[".ico"]		= "image/vnd.microsoft.icon";
	this->_mime_types[".ics"]		= "text/calendar";
	this->_mime_types[".jar"]		= "application/java-archive";
	this->_mime_types[".jpeg"]		= "image/jpeg";
	this->_mime_types[".jpg"]		= "image/jpeg";
	this->_mime_types[".js"]		= "text/javascript";
	this->_mime_types[".json"]		= "application/json";
	this->_mime_types[".jsonld"]	= "application/ld+json";
	this->_mime_types[".mid"]		= "audio/midi";
	this->_mime_types[".midi"]		= "audio/midi";
	this->_mime_types[".mjs"] 		= "text/javascript";
	this->_mime_types[".mp3"] 		= "audio/mpeg";
	this->_mime_types[".mp4"] 		= "video/mp4";
	this->_mime_types[".mpeg"] 		= "video/mpeg";
	this->_mime_types[".mpkg"]		= "application/vnd.apple.installer+xml";
	this->_mime_types[".odp"]		= "application/vnd.oasis.opendocument.presentation";
	this->_mime_types[".ods"]		= "application/vnd.oasis.opendocument.spreadsheet";
	this->_mime_types[".odt"]		= "application/vnd.oasis.opendocument.text";
	this->_mime_types[".oga"]		= "audio/ogg";
	this->_mime_types[".ogv"]		= "video/ogg";
	this->_mime_types[".ogx"]		= "application/ogg";
	this->_mime_types[".opus"]		= "audio/opus";
	this->_mime_types[".otf"]		= "font/otf";
	this->_mime_types[".png"]		= "image/png";
	this->_mime_types[".pdf"]		= "application/pdf";
	this->_mime_types[".php"]		= "application/x-httpd-php";
	this->_mime_types[".ppt"]		= "application/vnd.ms-powerpoint";
	this->_mime_types[".pptx"]		= "application/vnd.openxmlformats-officedocument.presentationml.presentation";
	this->_mime_types[".rar"]		= "application/vnd.rar";
	this->_mime_types[".rtf"]		= "application/rtf";
	this->_mime_types[".sh"]		= "application/x-sh";
	this->_mime_types[".svg"]		= "image/svg+xml";
	this->_mime_types[".tar"]		= "application/x-tar";
	this->_mime_types[".tif"]		= "image/tiff";
	this->_mime_types[".tiff"]		= "image/tiff";
	this->_mime_types[".ts"]		= "video/mp2t";
	this->_mime_types[".ttf"]		= "font/ttf";
	this->_mime_types[".txt"]		= "text/plain";
	this->_mime_types[".vsd"]		= "application/vnd.visio";
	this->_mime_types[".wav"]		= "audio/wav";
	this->_mime_types[".weba"]		= "audio/webm";
	this->_mime_types[".webm"]		= "video/webm";
	this->_mime_types[".webp"]		= "image/webp";
	this->_mime_types[".woff"]		= "font/woff";
	this->_mime_types[".woff2"]		= "font/woff2";
	this->_mime_types[".xhtml"]		= "application/xhtml+xml";
	this->_mime_types[".xls"]		= "application/vnd.ms-excel";
	this->_mime_types[".xlsx"]		= "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
	this->_mime_types[".xml"]		= "application/xml";
	this->_mime_types[".xul"]		= "application/vnd.mozilla.xul+xml";
	this->_mime_types[".zip"]		= "application/zip";
	this->_mime_types[".3gp"]		= "video/3gpp";
	this->_mime_types[".3g2"]		= "video/3gpp2";
	this->_mime_types[".7z"]		= "application/x-7z-compressed";

	return ;
}

} /** namespace webserv */
