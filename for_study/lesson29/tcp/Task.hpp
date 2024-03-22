#pragma once
#include <cassert>
#include <cstdio>
#include <pthread.h>
#include <iostream>
#include <unistd.h>

namespace ns_task
{
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

    class Task
    {
    public:
        void operator()()
        {
            Run();
        }

    private:
        int _sockfd;

    public:
        Task() = default;
        Task(int sockfd)
            : _sockfd(sockfd) {}

        void Run()
        {
            IOService(_sockfd);
            close(_sockfd);
        }

        ~Task() {}
    };
} // namespace ns_task
