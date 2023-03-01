/** INCLUDES ----------------------------------- */

# include <utils/utils.h>

/** METHODS ------------------------------------ */

namespace webserv {

namespace utils {

std::string sanitizePath( std::string path ) {
  std::string::iterator it = path.begin();
  while ( path.end() != it ) {
    if ( '/' == *it ) {
      ++it;
      while ( '/' == *it && path.end() != it ) {
        it = path.erase( it );
      }
    } else {
      ++it;
    }
  }
  return ( path );
}

} /** namespace utils */

} /** namespace webserv */
