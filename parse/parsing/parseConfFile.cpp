
#include "ServerConfig.hpp"
#include <iostream>
#include <fstream>
#include <cctype>

bool	checkConfFile(char *filename)
{
	std::ifstream	file(filename);

	if (!file.is_open())
	{
		std::cerr << "Error: could not open file" << std::endl;
		return false;
	}
	return true;
}

static void	parseServerdata(ServerConfig &server, std::string line, int *i)
{
	std::string data[7] = {"listen", "server_name", "host", "root", "client_max_body_size", "index", "error_page"};
	int			set = -1;
	int			code = 0;

	for (int j = 0; j < 7; j++)
	{
		if (line.compare(*i, data[j].length(), data[j]) == 0)
		{
			set = j;
			break ;
		}
	}
	switch (set)
	{
		case 0:
			*i += 6;
			while (std::isspace(line[*i]))
				*i += 1;
			server.setListenPort(line.substr(*i, line.find_first_of(";") - *i));
			break ;
		case 1:
			*i += 11;
			while (std::isspace(line[*i]))
				*i += 1;
			server.setServerName(line.substr(*i, line.find_first_of(";") - *i));
			break ;
		case 2:
			*i += 4;
			while (std::isspace(line[*i]))
				*i += 1;
			server.setHost(line.substr(*i, line.find_first_of(";") - *i));
			break ;
		case 3:
			*i += 4;
			while (std::isspace(line[*i]))
				*i += 1;
			server.setRoot(line.substr(*i, line.find_first_of(";") - *i));
			break ;
		case 4:
			*i += 21;
			while (std::isspace(line[*i]))
				*i += 1;
			server.setClientMaxBodySize(std::stoi(line.substr(*i, line.find_first_of(";") - *i)));
			break ;
		case 5:
			*i += 5;
			while (std::isspace(line[*i]))
				*i += 1;
			server.setIndex(line.substr(*i, line.find_first_of(";") - *i));
			break ;
		case 6:
			*i += 10;
			while (std::isspace(line[*i]))
				*i += 1;
			code = std::stoi(line.substr(*i, line.find_first_of(" ") - *i));
			*i += 3;
			while (std::isspace(line[*i]))
				*i += 1;
			server.addErrorPage(code, line.substr(*i, line.find_first_of(";") - *i));
			break ;
		default:
			break ;
	}
}

// static void	parseLocationData(LocationConfig &location, std::string line, int *i)
// {
// }

void	parseData(char *filename, ServerConfig &server, LocationConfig &location)
{
	std::ifstream	file(filename);
	std::string		line;
	bool			server_block = false;

	(void)location;
	(void)server;
	if (!file.is_open())
	{
		std::cerr << "Error: could not open file" << std::endl;
		return ;
	}
	while (std::getline(file, line))
	{
		if (line.empty())
			continue ;
		std::string temp;
		line = line.substr(line.find_first_not_of(" \t"), line.find_last_not_of(" \t") - line.find_first_not_of(" \t") + 1);
		std::cout << line << std::endl;
		if (line.compare(0, 1, "}") == 0)
			server_block = false;
		if (line.compare(0, 6, "server") == 0)
			server_block = true;
		int i = 0;
		if (server_block)
		{
			parseServerdata(server, line, &i);
			// parseLocationData(location, line, &i);
		}
	}
}
