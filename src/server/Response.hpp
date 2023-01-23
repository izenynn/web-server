#pragma once

#ifndef __RESPONSE_HPP__
# define __RESPONSE_HPP__

/** INCLUDES ----------------------------------- */

# include <map>
# include <string>

/** CLASS -------------------------------------- */

namespace webserv {

class Response {
	public:
	private:
		std::map<int, std::string>			_status_codes;
		std::map<std::string, std::string>	_mime_types;
}

} /** namespace webserv */

#endif /** __RESPONSE_HPP__ */
