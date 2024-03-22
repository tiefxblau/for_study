#pragma once
#include <iostream>
#include <string>
#include <unordered_map>
#include <sys/epoll.h>

class Event;
class Reactor;

class CallBack
{
public:
    virtual int operator()(Event& evp);
};

class Event
{
public:
    int fd = -1;
    std::string inbuffer;
    std::string outbuffer;

    CallBack* recver = nullptr;
    CallBack* sender = nullptr;
    CallBack* errorer = nullptr;

    Reactor* R = nullptr;

    template <class Recver, class Sender, class Errorer>
    void RegisterCallBack(const Recver& _recver, const Sender& _sender, const Errorer& _errorer)
    {
        recver = new Recver(_recver);
        sender = new Sender(_sender);
        errorer = new Errorer(_errorer);
    }
    void DeleteCallBack()
    {
        delete recver;
        delete sender;
        delete errorer;
    }
};

int CallBack::operator()(Event& evp)
{
    // std::cout << evp.fd << " none" << std::endl;
    return 0;
}

const int NUM = 1024;
class Reactor
{
private:
    int epfd;
    std::unordered_map<int, Event> events;
public:
    Reactor()
    {
        epfd = epoll_create(1);
        if (epfd < 0)
        {
            std::cerr << "epoll create failed" << std::endl;
            exit(1);
        }
    }

    bool InsertEvent(const Event& ev, uint32_t ep_events)
    {
        epoll_event ep;
        ep.events = ep_events;
        ep.data.fd = ev.fd;
        if (epoll_ctl(epfd, EPOLL_CTL_ADD, ev.fd, &ep) < 0)
        {
            std::cerr << "epoll add failed" << std::endl;
            return false;
        }

        events[ev.fd] = ev;
        std::cout << "已添加fd: " << ev.fd << std::endl;
        return true;
    }
    bool DeleteEvent(int fd)
    {
        if (epoll_ctl(epfd, EPOLL_CTL_DEL, fd, nullptr) < 0)
        {
            std::cerr << "epoll delete failed" << std::endl;
            return false;
        }

        auto it = events.find(fd);
        if (it != events.end())
        {
            it->second.DeleteCallBack();
            events.erase(fd);
        }
        std::cout << "已删除fd: " << fd << std::endl;
        return true;
    }
    bool ModifyEvent(int fd, uint32_t ep_events)
    {
        epoll_event ep;
        ep.events = ep_events;
        ep.data.fd = fd;
        if (epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ep) < 0)
        {
            std::cerr << "epoll modify failed" << std::endl;
            return false;
        }

        std::cout << "已修改fd: " << fd << std::endl;
        return true;
    }

    void Dispatcher()
    {
        while (true)
        {
            epoll_event eps[NUM];
            int n = epoll_wait(epfd, eps, NUM, -1);
            // std::cout << "epoll wait..." << std::endl;
            if (n < 0)
            {
                std::cerr << "epoll wait failed" << std::endl;
                exit(2);
            }
            else if (n == 0)
            {
                std::cout << "epoll wait timeout" << std::endl;
            }
            else
            {
                std::cout << "已经有fd就绪" << std::endl;
                for (int i = 0; i < n; i++)
                {
                    int sockfd = eps[i].data.fd;
                    uint32_t ep_events = eps[i].events;
                    if (ep_events & EPOLLERR)   ep_events |= EPOLLIN | EPOLLOUT;
                    if (ep_events & EPOLLHUP)   ep_events |= EPOLLIN | EPOLLOUT;

                    if (ep_events & EPOLLIN)
                    {
                        std::cout << "fd: " << sockfd << "读就绪" << std::endl; 
                        if (events[sockfd].recver)
                        {
                            CallBack& Recver = *(events[sockfd].recver);
                            Recver(events[sockfd]);
                        }
                        // std::cout << "完成读" << std::endl;
                    }

                    if (ep_events & EPOLLOUT)
                    {
                        std::cout << "fd: " << sockfd << "写就绪" << std::endl; 
                        if (events[sockfd].sender)
                        {
                            CallBack& Sender = *(events[sockfd].sender);
                            Sender(events[sockfd]);
                        }
                    }
                }
            }
        }
    }
};