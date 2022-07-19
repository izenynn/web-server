#include "../inc/Config.hpp"

Config::Config() {}

Config::Config(const Config& other) {}

Config::~Config() {}

Config& Config::operator=(const Config& other) {
	if (this == &other) return *this;
	return *this;
}

void Config::loadConfig(const std::string& file) {
	return;
}

const char* Form::GradeTooHighException::what() const throw() {
	return "Exception: grade too high";
}
