#include <iostream>
#include <string>
#include <cerrno>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

void Usage(char* proc)
{
    std::cout << "Usage:\n\t" << proc << " server_port" << std::endl;
}
void IOService(int sockfd)
{
    while (true)
    {
        char buffer[1024] = { 0 };
        ssize_t sz = read(sockfd, buffer, sizeof(buffer) - 1);
        if (sz > 0)
        {
            buffer[sz] = 0;
            std::cout << "client: " << buffer << std::endl;

            std::string msg("byebye");
            write(sockfd, msg.c_str(), msg.size());
        }
        else if (sz == 0)
        {
            std::cout << "client quit" << std::endl;
            break;
        }
    }
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        Usage(argv[0]);
        return 1;
    }

    //signal(SIGCHLD, SIG_IGN);
    int port = atoi(argv[1]);

    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0)
    {
        std::cout << "sock error: " << errno << std::endl;
        return 2;
    }

    sockaddr_in local;
    local.sin_family = AF_INET;
    local.sin_port = htons(port);
    local.sin_addr.s_addr = INADDR_ANY;

    if (bind(listenfd, (const sockaddr*)&local, sizeof(local)) < 0)
    {
        std::cout << "bind error: " << errno << std::endl;
        return 3;
    }

    if (listen(listenfd, 5) != 0)
    {
        std::cout << "listen error: " << errno << std::endl;
    }

    bool quit = false;
    while (!quit)
    {
        sockaddr_in peer;
        socklen_t len = sizeof(peer);

        int newfd = accept(listenfd, (sockaddr*)&peer, &len);
        if (newfd >= 0)
        {
            std::string cli_ip = inet_ntoa(peer.sin_addr);
            uint16_t cli_port = ntohs(peer.sin_port);

            std::cout << "get a new link: [" << cli_ip << ":" << cli_port << "] " << newfd << std::endl;

            pid_t pid = fork();
            if (pid > 0)
            {
                if (fork() > 0)
                    exit(0); 

                close(listenfd);
                IOService(newfd);
                close(newfd);

                exit(0);
            }
            else if (pid == 0)
            {
                waitpid(pid, nullptr, 0);
                close(newfd);
            }

        }
    }

    return 0;
}