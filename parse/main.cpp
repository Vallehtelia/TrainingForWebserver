#include "./parsing/ServerConfig.hpp"

int	main(int ac, char **av)
{
	std::vector<ServerConfig>	server;

	if (ac != 2)
	{
		std::cerr << "Error: wrong number of arguments" << std::endl;
		return 1;
	}
	checkConfFile(av[1]);
	std::cout << "Parsing file: " << av[1] << std::endl;
	parseData(av[1], server);
	for (std::vector<ServerConfig>::iterator it = server.begin(); it != server.end(); it++)
	{
		it->printConfig();
	}
	return 0;
}
