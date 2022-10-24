#ifndef CONFIG_HPP
# define CONFIG_HPP

# include <string>
# include <vector>
# include <fstream>

class Config {
	public:
		Config( void ); // default path
		Config( const std::string& path );
		virtual ~Config( void );

		Config								&operator=( const Config& other );

		// get
		const std::vector<ConfigServer>&	getServers( void ) const;
	private:
		int									load( const std::string& file );

		std::vector<ConfigServer>			_servers;
};

#endif
