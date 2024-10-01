
#include "socket.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <cerrno>
#include <fcntl.h>

int main()
{
	Socket	socket1;

	struct sockaddr_in	serv_addr;
	struct sockaddr_in	client_addr;
	socklen_t			client_len;
	int					client_fd;
	char				buffer[1024];
	size_t				bytes_recieved;

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(8080);
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	socket1.setSocketFd(socket(AF_INET, SOCK_STREAM, 0));
	if (socket1.getSocketFd() <= 0)
	{
		std::cout << "Failed to set socket: " << strerror(errno) << "\n";
		return 1;
	}
	int opt = 1;
	if (setsockopt(socket1.getSocketFd(), SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
	{
		std::cout << "setsockopt failed: " << strerror(errno) << "\n";
		return 1;
	}
	if (bind(socket1.getSocketFd(), (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
	{
		std::cout << "Failed to bind: " << strerror(errno) << "\n";
		return 1;
	}
	int sockfd = socket1.getSocketFd();

	if (listen(sockfd, 10) < 0)
	{
		std::cout << "Failed to listen: " << strerror(errno) << "\n";
		close(sockfd);
		return 1;
	}
	client_len = sizeof(client_addr);
	client_fd = accept(socket1.getSocketFd(), (struct sockaddr*)&client_addr, &client_len);
	if (client_fd < 0)
	{
		std::cout << "Failed to create client fd: " << strerror(errno) << "\n";
		close(sockfd);
		return 1;
	}
	const char *dataToSend = "what is going on here\n";
	if (send(client_fd, dataToSend, strlen(dataToSend), 0) < 0)
	{
		std::cout << "Failed to send: " << strerror(errno) << "\n";
		close(sockfd);
		return 1;
	}
	while (1)
	{
		bytes_recieved = recv(client_fd, buffer, sizeof(buffer), 0);
		if (bytes_recieved < 0)
		{
			std::cout << "Failed to recv: " << strerror(errno) << "\n";
			close(sockfd);
			return 1;
		}
		if (!strncmp(buffer, "exit", 4))
			break;
		std::cout << buffer << std::endl;
		bzero(buffer, strlen(buffer));
	}
	close(client_fd);
	close(sockfd);
	return (0);
}
