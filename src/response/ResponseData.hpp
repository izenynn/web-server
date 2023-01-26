#pragma once

#ifndef __RESPONSE_DATA_HPP__
# define __RESPONSE_DATA_HPP__

/** INCLUDES ----------------------------------- */

# include <string>
# include <vector>
# include <fcntl.h> // open()
# include <unistd.h> // close()
# include <sys/stat.h> // stat()
# include <stdlib.h> // malloc()
# include <dirent.h> // opendir()

/** CLASS -------------------------------------- */

namespace webserv {

class ResponseData {
	public:
		ResponseData( void );
		ResponseData( const std::string & path );
		~ResponseData( void );

		static const size_t kReadBuffer;

		void print( void ) const;

		void setPath( const std::string & path );

		bool openFile();
		//bool createFile(...);
		//bool appendFile(...);
		//bool deleteFile(...);
		void closeFile();

		const std::string getIndex( std::vector<std::string> & indexes );
		//void getFile( void ); // FIXME remove this line!!!

		bool isDirectory( void ) const;
		bool fileExists( void ) const;

		std::string getAutoIndex( const std::string & uri );
		const std::string & getExtension( void );
		const std::string getFileContent( void );
	private:
		ResponseData( const ResponseData & other ); // not necessary
		ResponseData & operator=( const ResponseData & other ); // not necessary

		void parseFileName( void );

		std::string		_path;

		int				_fd;
		std::string		_fileName;
		std::string		_fileExtension;
};

} /** namespace webserv */

#endif /** __RESPONSE_DATA_HPP__ */
