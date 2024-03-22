#include <iostream>
#include <string>
#include <unistd.h>
#include "Socket.hpp"

using namespace std;

int main()
{
    int sockfd = Sock::Socket();
    Sock::Connect(sockfd, "101.43.183.8", 8080);

    while (true)
    {
        string msg;
        cin >> msg;
        write(sockfd, msg.c_str(), msg.size());
    }

    return 0;
}