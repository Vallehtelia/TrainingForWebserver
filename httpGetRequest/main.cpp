
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
	char				buffer[1024];
	size_t				bytes_recieved;

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(8080);

	if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
	{
		std::cout << "Invalid address or address not supported.\n";
		return 1;
	}
	socket1.setSocketFd(socket(AF_INET, SOCK_STREAM, 0));
	if (socket1.getSocketFd() <= 0)
	{
		std::cout << "Failed to set socket: " << strerror(errno) << "\n";
		return 1;
	}
	if (connect(socket1.getSocketFd(), (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
	{
		std::cout << "Connection failed " << strerror(errno) << std::endl;
		return 1;
	}
	const char *httpRequest =
		"GET / HTTP/1.1\r\n"
		"Host: localhost\r\n"
		"Connection: close\r\n\r\n";

	if (send(socket1.getSocketFd(), httpRequest, strlen(httpRequest), 0) < 0)
	{
		std::cout << "Failed to send request: " << strerror(errno) << "\n";
		return 1;
	}
	while ((bytes_recieved = recv(socket1.getSocketFd(), buffer, sizeof(buffer) - 1, 0)) > 0)
	{
		buffer[bytes_recieved] = '\0';
		std::cout << buffer;
	}

	if (bytes_recieved < 0)
	{
		std::cout << "Failed to recieve data: " << strerror(errno) << "\n";
	}

	close(socket1.getSocketFd());
	return (0);
}
