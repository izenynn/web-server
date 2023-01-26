#pragma once

#ifndef __LOG_HPP__
# define __LOG_HPP__

/** INCLUDES ----------------------------------- */

# include <iostream>
# include <string>

# include <sstream>
# define SSTR( x ) static_cast< std::ostringstream & >( ( std::ostringstream() << std::dec << x ) ).str()

/** COLORS ------------------------------------- */

// reset
# define C_OFF			"\033[0m"

// colors
# define C_BLACK		"\033[0;30m"
# define C_BLUE			"\033[0;34m"
# define C_CYAN			"\033[0;36m"
# define C_GREEN		"\033[0;32m"
# define C_PURPLE		"\033[0;35m"
# define C_RED			"\033[0;31m"
# define C_YELLOW		"\033[0;33m"
# define C_WHITE		"\033[0;37m"

# define C_DARKGREY		"\033[0;90m"
# define C_LIGHTBLUE	"\033[0;94m"
# define C_LIGHTCYAN	"\033[0;96m"
# define C_LIGHTGREEN	"\033[0;92m"
# define C_LIGHTPURPLE	"\033[0;95m"
# define C_LIGHTRED		"\033[0;91m"
# define C_LIGHTYELLOW	"\033[0;93m"

// bold
# define CB_BLACK		"\033[1;30m"
# define CB_BLUE		"\033[1;34m"
# define CB_CYAN		"\033[1;36m"
# define CB_GREEN		"\033[1;32m"
# define CB_PURPLE		"\033[1;35m"
# define CB_RED			"\033[1;31m"
# define CB_YELLOW		"\033[1;33m"
# define CB_WHITE		"\033[1;37m"

// underlained
# define CU_BLACK		"\033[4;30m"
# define CU_BLUE		"\033[4;34m"
# define CU_CYAN		"\033[4;36m"
# define CU_GREEN		"\033[4;32m"
# define CU_PURPLE		"\033[4;35m"
# define CU_RED			"\033[4;31m"
# define CU_YELLOW		"\033[4;33m"
# define CU_WHITE		"\033[4;37m"

// background white
# define CBACK_BLACK_BLACK		"\033[30;40m"
# define CBACK_BLACK_BLUE		"\033[34;40m"
# define CBACK_BLACK_CYAN		"\033[36;40m"
# define CBACK_BLACK_GREEN		"\033[32;40m"
# define CBACK_BLACK_PURPLE		"\033[35;40m"
# define CBACK_BLACK_RED		"\033[31;40m"
# define CBACK_BLACK_YELLOW		"\033[33;40m"
# define CBACK_BLACK_WHITE		"\033[37;40m"

// background blue
# define CBACK_BLUE_BLACK		"\033[30;44m"
# define CBACK_BLUE_BLUE		"\033[34;44m"
# define CBACK_BLUE_CYAN		"\033[36;44m"
# define CBACK_BLUE_GREEN		"\033[32;44m"
# define CBACK_BLUE_PURPLE		"\033[35;44m"
# define CBACK_BLUE_RED			"\033[31;44m"
# define CBACK_BLUE_YELLOW		"\033[33;44m"
# define CBACK_BLUE_WHITE		"\033[37;44m"

// background cyan
# define CBACK_CYAN_BLACK		"\033[30;46m"
# define CBACK_CYAN_BLUE		"\033[34;46m"
# define CBACK_CYAN_CYAN		"\033[36;46m"
# define CBACK_CYAN_GREEN		"\033[32;46m"
# define CBACK_CYAN_PURPLE		"\033[35;46m"
# define CBACK_CYAN_RED			"\033[31;46m"
# define CBACK_CYAN_YELLOW		"\033[33;46m"
# define CBACK_CYAN_WHITE		"\033[37;46m"

// background green
# define CBACK_GREEN_BLACK		"\033[30;42m"
# define CBACK_GREEN_BLUE		"\033[34;42m"
# define CBACK_GREEN_CYAN		"\033[36;42m"
# define CBACK_GREEN_GREEN		"\033[32;42m"
# define CBACK_GREEN_PURPLE		"\033[35;42m"
# define CBACK_GREEN_RED		"\033[31;42m"
# define CBACK_GREEN_YELLOW		"\033[33;42m"
# define CBACK_GREEN_WHITE		"\033[37;42m"

// background purple
# define CBACK_PURPLE_BLACK		"\033[30;45m"
# define CBACK_PURPLE_BLUE		"\033[34;45m"
# define CBACK_PURPLE_CYAN		"\033[36;45m"
# define CBACK_PURPLE_GREEN		"\033[32;45m"
# define CBACK_PURPLE_PURPLE	"\033[35;45m"
# define CBACK_PURPLE_RED		"\033[31;45m"
# define CBACK_PURPLE_YELLOW	"\033[33;45m"
# define CBACK_PURPLE_WHITE		"\033[37;45m"

// background red
# define CBACK_RED_BLACK		"\033[30;41m"
# define CBACK_RED_BLUE			"\033[34;41m"
# define CBACK_RED_CYAN			"\033[36;41m"
# define CBACK_RED_GREEN		"\033[32;41m"
# define CBACK_RED_PURPLE		"\033[35;41m"
# define CBACK_RED_RED			"\033[31;41m"
# define CBACK_RED_YELLOW		"\033[33;41m"
# define CBACK_RED_WHITE		"\033[37;41m"

// background white
# define CBACK_WHITE_BLACK		"\033[30;47m"
# define CBACK_WHITE_BLUE		"\033[34;47m"
# define CBACK_WHITE_CYAN		"\033[36;47m"
# define CBACK_WHITE_GREEN		"\033[32;47m"
# define CBACK_WHITE_PURPLE		"\033[35;47m"
# define CBACK_WHITE_RED		"\033[31;47m"
# define CBACK_WHITE_YELLOW		"\033[33;47m"
# define CBACK_WHITE_WHITE		"\033[37;47m"

// background yellow
# define CBACK_YELLOW_BLACK		"\033[30;43m"
# define CBACK_YELLOW_BLUE		"\033[34;43m"
# define CBACK_YELLOW_CYAN		"\033[36;43m"
# define CBACK_YELLOW_GREEN		"\033[32;43m"
# define CBACK_YELLOW_PURPLE	"\033[35;43m"
# define CBACK_YELLOW_RED		"\033[31;43m"
# define CBACK_YELLOW_YELLOW	"\033[33;43m"
# define CBACK_YELLOW_WHITE		"\033[37;43m"

/** METHODS ------------------------------------ */

namespace webserv {

namespace colors {

} /** namespace colors */

namespace log {
	inline void info( const std::string & msg ) {
		std::cout << "[" << C_BLUE << "*" << C_OFF << "] " << msg << std::endl;
		return;
	}
	inline void success( const std::string & msg) {
		std::cout << "[" << C_GREEN << "+" << C_OFF << "] " << msg << std::endl;
		return;
	}
	inline void warning( const std::string & msg) {
		std::cout << "[" << C_YELLOW << "!" << C_OFF << "] " << msg << std::endl;
		return;
	}
	inline void failure( const std::string & msg) {
		std::cerr << "[" << C_RED << "-" << C_OFF << "] " << msg << std::endl;
		return;
	}
	inline void error( const std::string & msg) {
		std::clog << "[" << CBACK_RED_WHITE << "ERROR" << C_OFF << "] " << msg << std::endl;
		return;
	}
	inline void debug( const std::string & msg ) {
		std::clog << "[" << CBACK_YELLOW_WHITE << "DEBUG" << C_OFF << "] " << msg << std::endl;
		return;
	}
} /** namespace log */

} /** namespace webserv */

#endif /** __LOG_HPP__ */
