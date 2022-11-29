#ifndef __CONFIG_HPP__
# define __CONFIG_HPP__

# include <string>
# include <vector>
# include <fstream>

class Config {
	public:
		Config( const char* path ); // config class needs a config file
		virtual ~Config( void );

		//const std::vector<ConfigServer>&	getServers( void ) const;
	private:
		Config( void ); // not necessary
		Config &operator=( const Config& other ); // not necessary
		//std::vector<ConfigServer>			_servers;

		void load( const char* file );
	public:
		class ExtraClosingBrackets : public std::exception {
			public: const char* what() const throw();
		};
		class ExtraOpeningBrackets : public std::exception {
			public: const char* what() const throw();
		};
};

#endif // __CONFIG_HPP__
