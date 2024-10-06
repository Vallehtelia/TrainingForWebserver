#include "socket.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <cerrno>
#include <fcntl.h>
#include <iostream>
#include <fstream>
#include <map>

std::string get_content_type(const std::string& path) {
    // Map of common file extensions and their MIME types
    std::map<std::string, std::string> mime_types = {
        {".html", "text/html"},
        {".css", "text/css"},
        {".js", "application/javascript"},
        {".jpg", "image/jpeg"},
        {".jpeg", "image/jpeg"},
        {".png", "image/png"},
        {".gif", "image/gif"}
    };

    // Find the file extension in the path
    size_t dot_pos = path.find_last_of(".");
    if (dot_pos != std::string::npos) {
        std::string ext = path.substr(dot_pos);
        if (mime_types.count(ext)) {
            return mime_types[ext];
        }
    }

    // Default to plain text if no recognized extension is found
    return "text/plain";
}

int main() {
    Socket socket1;
    struct sockaddr_in serv_addr, client_addr;
    socklen_t client_len;
    int client_fd;
    char buffer[1024];
    size_t bytes_received;

    // Configure server address and port
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    // Create socket
    socket1.setSocketFd(socket(AF_INET, SOCK_STREAM, 0));
    if (socket1.getSocketFd() <= 0) {
        std::cout << "Failed to set socket: " << strerror(errno) << "\n";
        return 1;
    }

    // Set socket options to reuse address
    int opt = 1;
    if (setsockopt(socket1.getSocketFd(), SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        std::cout << "setsockopt failed: " << strerror(errno) << "\n";
        return 1;
    }

    // Bind socket to address and port
    if (bind(socket1.getSocketFd(), (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cout << "Failed to bind: " << strerror(errno) << "\n";
        return 1;
    }

    // Listen for connections
    if (listen(socket1.getSocketFd(), 10) < 0) {
        std::cout << "Failed to listen: " << strerror(errno) << "\n";
        close(socket1.getSocketFd());
        return 1;
    }

    std::cout << "Server is listening on port 8080...\n";

    while (true) {
        // Accept incoming connection
        client_len = sizeof(client_addr);
        client_fd = accept(socket1.getSocketFd(), (struct sockaddr*)&client_addr, &client_len);
        if (client_fd < 0) {
            std::cout << "Failed to create client fd: " << strerror(errno) << "\n";
            close(socket1.getSocketFd());
            return 1;
        }

        // Tama vastaanottaa kutsun selaimelta bufferiin, bufferi pitaa parsea ja toimia kutsun mukaan
        bytes_received = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received < 0) {
            std::cout << "Failed to receive: " << strerror(errno) << "\n";
            close(socket1.getSocketFd());
            return 1;
        }
        buffer[bytes_received] = '\0';
        // Tassa bufferi nyt vaan printataan terminaaliin
        std::cout << "Received request:\n" << buffer << std::endl;

        // Tassa luetaan html filu ja asetetaan oikee path mika lahetetaan clientille
        char *page = (strstr(buffer, "GET") + 4);
        int i = 0;
        while (page[i] != ' ' && page[i] != '\0')
            i++;
        page[i] = '\0';
        std::string path = "./html";
        if (strcmp(page, "/") == 0)
            path += "/index.html";
        else
            path += page; // Append the requested file path

        std::cout << "Serving file: " << path << std::endl;

        std::ifstream file(path);
        if (!file.is_open())
        {// If the file is not found, serve a 404 error
            const char *not_found_response =
                "HTTP/1.1 404 Not Found\r\n"
                "Content-Type: text/html\r\n"
                "Content-Length: 45\r\n"
                "Connection: close\r\n\r\n"
                "<html><body><h1>404 Not Found</h1></body></html>";
            send(client_fd, not_found_response, strlen(not_found_response), 0);
            close(client_fd);
            continue ;
        }

        std::string content_type = get_content_type(path);

        std::string html_content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        file.close();
        std::cout << "Content type: " << content_type << "\n";
        // Prepare the HTTP response headers
        std::string http_response =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: " + content_type + "\r\n"
            "Content-Length: " + std::to_string(html_content.length()) + "\r\n"
            "Connection: close\r\n\r\n" + html_content;

        // Lahetetaan tiedot takaisin selaimeen
        if (send(client_fd, http_response.c_str(), http_response.length(), 0) < 0) {
            std::cout << "Failed to send: " << strerror(errno) << "\n";
            close(client_fd);
            close(socket1.getSocketFd());
            return 1;
        }

        // Close the client socket
        close(client_fd);
        std::cout << "Server closed connection.\n";
    }

    // Close server socket
    close(socket1.getSocketFd());
    return 0;
}
