#ifndef CONFIG_HPP
# define CONFIG_HPP

# include <string>
# include <vector>
# include <fstream>


// FIXME remove \/
# include <iostream>

class Config {
	public:
		Config( void ); // default path
		Config( const std::string& path );
		virtual ~Config( void );

		Config								&operator=( const Config& other );

		// get
		//const std::vector<ConfigServer>&	getServers( void ) const;

		int									load( const std::string& file );
	private:
		//std::vector<ConfigServer>			_servers;

	public:
		class ExtraClosingBrackets : public std::exception {
			public: const char* what() const throw();
		};
		class ExtraOpeningBrackets : public std::exception {
			public: const char* what() const throw();
		};
};

#endif
