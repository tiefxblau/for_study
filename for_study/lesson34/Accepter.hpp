#pragma once
#include "Reactor.hpp"
#include "Service.hpp"
#include "Socket.hpp"
#include "Util.hpp"

class Accepter : public CallBack
{
public:
    virtual int operator()(Event& ev)
    {
        int listen_sock = ev.fd;
        std::cout << "listenfd: " << listen_sock << "正在获取新的链接" << std::endl;
        
        int new_fd;
        while ((new_fd = Sock::Accept(listen_sock)) >= 0)
        {
            Event new_ev;
            new_ev.fd = new_fd;
            new_ev.R = ev.R;

            new_ev.RegisterCallBack(Recver(), Sender(), Errorer());

            ev.R->InsertEvent(std::move(new_ev), EPOLLIN | EPOLLET);
            SetNoBlock(new_fd);
            std::cout << "已获取新的fd: " << new_fd << std::endl;
        }

        return 0;
    }
};