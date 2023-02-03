#include <iostream>

extern char** environ;

int main(int argc, char* argv[]) {
	std::cout << "Content-type:text/html\r\n\r\n";
	std::cout << "<html>\n";
	std::cout << "<head>\n";
	std::cout << "<title>CGI Program</title>\n";
	std::cout << "</head>\n";

	std::cout << "<body>\n";
	std::cout << "<h2>Hello World! This is a CGI program in C++</h2>\n";
	std::cout << "<br />\n";

	std::cout << "<h2>Arguments:</h2>\n";
	for ( int i = 0; i < argc; ++i) {
		std::cout << "<p>" << argv[i] << "</p>\n";
	}
	std::cout << "<h2>Stdin:</h2>\n";
	for (std::string line; std::getline(std::cin, line);) {
		std::cout << "<p>" << line << "</p>" << std::endl;
	}
	std::cout << "<h2>Environment:</h2>\n";
	for ( char** i = environ; *i != NULL; ++i ) {
		std::cout << "<p>" << *i << "</p>\n";
	}

	std::cout << "</body>\n";
	std::cout << "</html>\n";

	return ( 0 );
}
