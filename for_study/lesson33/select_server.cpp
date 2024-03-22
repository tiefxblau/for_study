#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include "Socket.hpp"

using namespace std;

const int NUM = sizeof(fd_set) * 8;

void Usage()
{
    cout << "Usage:\n\t" << "./select_server server_port" << endl;
    cout << "example: ./select_server 8080" << endl;
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        Usage();
        return 1;
    }
    int port = atoi(argv[1]);

    int listen_sock = Sock::Socket();
    Sock::Bind(listen_sock, port);
    Sock::Listen(listen_sock);

    vector<int> fd_array(NUM, -1);
    fd_array[0] = listen_sock;

    while (true)
    {
        fd_set readset;
        FD_ZERO(&readset);
        int max_fd = 0;
        for (int i = 0; i < NUM; ++i)
        {
            if (fd_array[i] == -1)
                continue;

            FD_SET(fd_array[i], &readset);
            if (max_fd < fd_array[i])
                max_fd = fd_array[i];
        }

        // timeval time = { 5, 0 };
        int ret = select(max_fd + 1, &readset, nullptr, nullptr, nullptr);

        switch (ret)
        {
        case -1:
            cout << "select error" << endl;
            return 2;
            break;
        case 0:
            cout << "select timeout" << endl;
            break;
        default:
            cout << "已经有fd就绪" << endl;

            if (FD_ISSET(fd_array[0], &readset))
            {
                cout << "listen_fd就绪, 获得新的连接" << endl;
                int sockfd = Sock::Accept(fd_array[0]);
                int pos = 1;
                for (pos = 1; pos < NUM; pos++)
                {
                    if (fd_array[pos] == -1)
                        break;
                }

                if (pos < NUM)
                {
                    fd_array[pos] = sockfd;
                    cout << "已将新的fd放入fd_array[" << pos << "]中" << endl;
                }
                else
                {
                    close(sockfd);
                    cout << "服务器已满载" << endl;
                }
            }

            for (int pos = 1; pos < NUM; ++pos)
            {
                if (fd_array[pos] == -1)
                    continue;

                if (FD_ISSET(fd_array[pos], &readset))
                {
                    char buffer[1024];
                    int sz = read(fd_array[pos], buffer, sizeof(buffer) - 1);
                    if (sz > 0)
                    {
                        buffer[sz] = 0;
                        cout << "client[" << pos << "]# " << buffer << endl;
                    }
                    else if (sz == 0)
                    {
                        close(fd_array[pos]);
                        fd_array[pos] = -1;
                        cout << "client[" << pos << "] quit" << endl; 
                    }
                    else
                    {
                        cout << "read error" << endl;
                        close(fd_array[pos]);
                        fd_array[pos] = -1;
                    }
                }
            }

            break;
        }
    }
}