#include "./parsing/ServerConfig.hpp"

int	main(int ac, char **av)
{
	ServerConfig	server;
	LocationConfig	location;

	if (ac != 2)
	{
		std::cerr << "Error: wrong number of arguments" << std::endl;
		return 1;
	}
	checkConfFile(av[1]);
	std::cout << "Parsing file: " << av[1] << std::endl;
	parseData(av[1], server, location);
	server.printConfig();
	// location.printConfig();
	return 0;
}
