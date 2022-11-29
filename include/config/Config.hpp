#ifndef __CONFIG_HPP__
# define __CONFIG_HPP__

# include <string>
# include <vector>
# include <fstream>


// FIXME remove \/
# include <iostream>

class Config {
	public:
		Config( const std::string& path );
		virtual ~Config( void );

		Config								&operator=( const Config& other );

		int									load( const char* file );

		//const std::vector<ConfigServer>&	getServers( void ) const;
	private:
		Config( void ); // default path handled on 'Server' class
		//std::vector<ConfigServer>			_servers;

	public:
		class ExtraClosingBrackets : public std::exception {
			public: const char* what() const throw();
		};
		class ExtraOpeningBrackets : public std::exception {
			public: const char* what() const throw();
		};
};

#endif // __CONFIG_HPP__
