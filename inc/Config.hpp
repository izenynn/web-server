#pragma once

#ifndef CONFIG_HPP
# define CONFIG_HPP

class Config() {
	public:
		Config();
		Config(const Config& other);
		~Config();

		Config& operator=(const Config& other);

		void loadConfig(const std::string& file);
	private:
		static const DEFAULT_FILE_LOCATION = "./config.conf";
	public:
		class InvalidConfigException : public std::exception {
			public: const char* what() const throw();
		};
};

#endif
