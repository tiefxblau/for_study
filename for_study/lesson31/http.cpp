#include "Socket.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define WWWROOT "../wwwroot/"
#define HOME_PAGE "index.html"
#define SIZE (1024 * 10)

void HandlerHttpRequest(int* psockfd)
{
    int sockfd = *psockfd;
    delete psockfd;

    char buffer[SIZE] = { 0 };
    ssize_t sz = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
    if (sz > 0)
    {
        buffer[sz] = 0;
        std::string req = buffer;
        std::cout << req << std::cout;

    }

    std::string html_file = WWWROOT;
    html_file += HOME_PAGE;

    struct stat st;
    stat(html_file.c_str(), &st);

    std::string resp = "http/1.0 200 OK\n";
    resp += "Content-Type: text/html; charset=utf8\n";
    resp += "Content-Length: " + std::to_string(st.st_size) + "\n";
    resp += "\n";
    
    std::ifstream fin(html_file);
    std::string line;
    while (std::getline(fin, line))
    {
        resp += line;
    }

    send(sockfd, resp.c_str(), resp.size(), 0);
    close(sockfd);
}

int main()
{
    int listen_sockfd = Sock::Socket();
    Sock::Bind(listen_sockfd, 8080);
    Sock::Listen(listen_sockfd);

    while (true)
    {
        int sockfd = Sock::Accept(listen_sockfd);
        if (sockfd > 0)
        {
            int* pram = new int(sockfd);
            std::thread thd = std::thread(HandlerHttpRequest, pram);
            thd.detach();
        }
    }

    return 0;
}