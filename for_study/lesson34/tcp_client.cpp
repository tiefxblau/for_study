#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cerrno>
#include <unistd.h>
#include "Util.hpp"

void Usage(char* proc)
{
    std::cout << "Usage:\n\t" << proc << " server_ip server_port" << std::endl;
}
void IOService(int sockfd)
{
    bool quit = false;
    while (!quit)
    {
        std::string msg;
        std::cout << "请输入: ";
        std::cin >> msg;

        write(sockfd, msg.c_str(), msg.size());

        char buffer[1024] = { 0 };
        ssize_t sz = read(sockfd, buffer, sizeof(buffer) - 1);
        while (sz < 0)
        {
            sz = read(sockfd, buffer, sizeof(buffer) - 1);
            sleep(2);
        }
        if (sz > 0)
        {
            buffer[sz] = 0;
            std::cout << "server# " << buffer << std::endl;
        }
    }
}

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        Usage(argv[0]);
        return 1;
    }

    int port = atoi(argv[2]);
    std::string ip = argv[1];

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        std::cerr << "socket errer: " << errno << std::endl;
        return 2;
    }

    sockaddr_in dst;
    dst.sin_family = AF_INET;
    dst.sin_addr.s_addr = inet_addr(ip.c_str());
    dst.sin_port = htons(port);

    if (connect(sockfd, (const sockaddr*)&dst, sizeof(dst)) == -1)
    {
        std::cout << "connect error: " << errno << std::endl;
        return 4;
    }

    std::cout << "connect success" << std::endl;
    SetNoBlock(sockfd);

    IOService(sockfd);

    return 0;
}