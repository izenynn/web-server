#pragma once

#ifndef __URI_HPP__
# define __URI_HPP__

/** INCLUDES ----------------------------------- */

# include <string>
# include <vector>
# include <fcntl.h> // open()
# include <unistd.h> // close()
# include <sys/stat.h> // stat()
# include <stdlib.h> // malloc()

/** CLASS -------------------------------------- */

namespace webserv {

class Uri {
	public:
		Uri( void );
		Uri( const std::string & path );
		~Uri( void );

		static const size_t kReadBuffer;

		void setPath( const std::string & path );

		bool openFile();
		//bool createFile(...);
		//bool appendFile(...);
		//bool deleteFile(...);
		void closeFile();

		std::string getIndex( std::vector<std::string> & indexes );
		//void getFile( void ); // FIXME remove this line!!!

		bool isDirectory( void );
		bool fileExists( void );

		std::string getAutoIndex( const std::string & uri );
		const std::string & getExtension( void );
		const std::string getContent( void );
	private:
		Uri( const Uri & other ); // not necessary
		Uri & operator=( const Uri & other ); // not necessary

		void parseFileName( void );

		std::string		_path;

		int				_fd;
		std::string		_fileName;
		std::string		_fileExtension;
};

} /** namespace webserv */

#endif /** __URI_HPP__ */
