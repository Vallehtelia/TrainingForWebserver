
#include "LocationConfig.hpp"

LocationConfig::LocationConfig(): path("/"), autoindex(false), index("index.html"), redirect(""), cgi_path("")
{
	std::cout << "LocationConfig constructor" << std::endl;
}

LocationConfig::~LocationConfig()
{
	std::cout << "LocationConfig destructor" << std::endl;
}

LocationConfig::LocationConfig(const LocationConfig &other)
{
	std::cout << "LocationConfig copy constructor" << std::endl;
	*this = other;
}

LocationConfig &LocationConfig::operator=(const LocationConfig &other)
{
	std::cout << "LocationConfig assignation operator" << std::endl;
	if (this != &other)
	{
		path = other.path;
		allow_methods = other.allow_methods;
		autoindex = other.autoindex;
		index = other.index;
		redirect = other.redirect;
		cgi_path = other.cgi_path;
		cgi_ext = other.cgi_ext;
	}
	return *this;
}

void	LocationConfig::printConfig() const
{
	std::cout << "Location config:" << std::endl;
	std::cout << "Path: " << path << std::endl;
	std::cout << "Allow methods:" << std::endl;
	for (std::vector<std::string>::const_iterator it = allow_methods.begin(); it != allow_methods.end(); it++)
	{
		std::cout << *it << std::endl;
	}
	std::cout << "Autoindex: " << autoindex << std::endl;
	std::cout << "Index: " << index << std::endl;
	std::cout << "Redirect: " << redirect << std::endl;
	std::cout << "Cgi path: " << cgi_path << std::endl;
	std::cout << "Cgi extensions:" << std::endl;
	for (std::vector<std::string>::const_iterator it = cgi_ext.begin(); it != cgi_ext.end(); it++)
	{
		std::cout << *it << std::endl;
	}
}

std::string	LocationConfig::getLocation() const
{
	return path;
}

std::string	LocationConfig::getRoot() const
{
	return path;
}
