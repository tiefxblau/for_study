#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cerrno>

void Usage(char* proc)
{
    std::cout << "Usage:\n\t" << proc << " server_ip server_port" << std::endl;
}

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        Usage(argv[0]);
        return 1;
    }

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
        std::cerr << "socket errer: " << errno << std::endl;

    sockaddr_in dst;
    dst.sin_family = AF_INET;
    dst.sin_addr.s_addr = inet_addr(argv[1]);
    dst.sin_port = htons(atoi(argv[2]));

    bool quit = false;
    while (!quit)
    {
        std::string msg;
        std::cout << "请输入: ";
        std::cin >> msg;

        sendto(sockfd, msg.c_str(), msg.size(), 0, (const sockaddr*)&dst, sizeof(dst));

        char buffer[1024] = { 0 };
        ssize_t sz = recvfrom(sockfd, buffer, sizeof(buffer), 0, nullptr, nullptr);
        if (sz > 0)
        {
            buffer[sz] = 0;
            std::cout << "server# " << buffer << std::endl;
        }
    }

    return 0;
}