/**	INCLUDES ----------------------------------- */

#include <utils/log.hpp>

namespace webserv {
namespace log {
	void info(const std::string& msg) {
		std::cout << "[" << C_BLUE << "*" << C_OFF << "]" << msg << std::endl;
		return;
	}
	void success(const std::string& msg) {
		std::cout << "[" << C_GREEN << "+" << C_OFF << "]" << msg << std::endl;
		return;
	}
	void warning(const std::string& msg) {
		std::cout << "[" << C_YELLOW << "!" << C_OFF << "]" << msg << std::endl;
		return;
	}
	void failure(const std::string& msg) {
		std::cout << "[" << C_RED << "-" << C_OFF << "]" << msg << std::endl;
		return;
	}
	void error(const std::string& msg) {
		std::cout << "[" << CBACK_RED_WHITE << "ERROR" << C_OFF << "]" << msg << std::endl;
		return;
	}
} // namespace log
} // namespace webserv
