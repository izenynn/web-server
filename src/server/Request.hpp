#pragma once

#ifndef __REQUEST_HPP__
# define __REQUEST_HPP__

/** INCLUDES ----------------------------------- */

# include <string>
# include <map>
# include <sstream>
# include <sys/time.h>

# include <config/Config.hpp>

/** CLASS -------------------------------------- */

namespace webserv {

class Request {
	public:
		Request( void );
		~Request( void );

		void print( void );

		int parse( const std::string & buffer );

		const struct timeval & getTime( void );

		friend class RequestConfig;
	private:
		Request( const Request & other ); // not necessary
		Request & operator=( const Request & other ); // not necessary

		static const int kLimitRequestLimit;

		int parseRequestLine( void );
		int parseHeaders( void );
		int parseBody( void );
		int parseChunk( void );

		int parseChunkTrailer( void );

		std::string		_method;
		std::string		_request_uri;
		std::string		_uri_params;
		std::string		_version;
		std::string		_body;
		std::map<std::string, std::string> _headers;

		std::string		_buffer;

		std::string::size_type	_length;

		struct timeval _time;

		enum Status {
			kRequestLine = 0,
			kHeaders,
			kBody,
			kChunk,
			kComplete,
			kError
		};
		enum ChunkStatus {
			kChunkBody = 0,
			kChunkSize
		};

		Status			_status;
		ChunkStatus		_chunkStatus;
};

} /** namespace webserv */

#endif /** __REQUEST_HPP__ */
