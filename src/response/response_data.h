#ifndef WEBSERV_RESPONSEDATA_H_
# define WEBSERV_RESPONSEDATA_H_

/** INCLUDES ----------------------------------- */

# include <string>
# include <vector>
# include <map>

/** CLASS -------------------------------------- */

namespace webserv {

class ResponseData {
  public:
    ResponseData( void );
    ResponseData( const std::string & path );
    ~ResponseData( void );

    void print( void ) const;

    void setPath( const std::string & path );

    bool openFile();
    void createFile( const std::string & data );
    void appendFile( const std::string & data );
    void deleteFile( void );
    void closeFile( void );

    const std::string getIndex( const std::vector<std::string> & indexes );

    bool isDirectory( void ) const;
    bool fileExists( void ) const;

    std::string getAutoIndex( const std::string & uri );

    const std::string &   getExtension( void ) const;
    const std::string   getFileContent( void ) const;
    const std::string &   getPath(void) const;
  private:
    ResponseData( const ResponseData & other ); // not necessary
    ResponseData & operator=( const ResponseData & other ); // not necessary

    void parseFileName( void );

    std::string   _path;

    int       _fd;
    std::string   _fileName;
    std::string   _fileExtension;
};

} // namespace webserv

#endif // WEBSERV_RESPONSEDATA_H_
