#include <iostream>
#include <string>
#include <cerrno>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void Usage(char* proc)
{
    std::cout << "Usage:\n\t" << proc << " server_port" << std::endl;
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        Usage(argv[0]);
        return 1;
    }
    int port = atoi(argv[1]);

    int socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_fd < 0)
        std::cerr << "socket errer: " << errno << std::endl;

    sockaddr_in local;
    local.sin_family = AF_INET;
    local.sin_addr.s_addr = INADDR_ANY;
    local.sin_port = htons(port);

    if (bind(socket_fd, (const struct sockaddr*)&local, sizeof(local)) < 0)
        std::cerr << "bind errer: " << errno << std::endl;

    bool quit = false;
    while (!quit)
    {
        char buffer[1024] = { 0 };
        sockaddr_in src;
        socklen_t len = sizeof(src);
        ssize_t sz = recvfrom(socket_fd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr*)&src, &len);
        if (sz > 0)
        {
            buffer[sz] = 0;
            std::cout << "client$ " << buffer << std::endl;
            std::string msg = "hello";
            sendto(socket_fd, msg.c_str(), msg.size(), 0, (const sockaddr*)&src, len);
        }
        else if(sz == 0)
        {
            std::cout << "client quit" << std::endl;
        }
        
    }

    return 0;
}