#pragma once
#include "Reactor.hpp"
#include "Util.hpp"
#include <unistd.h>
#include <cerrno>

const int READ_NUM = 1024;
const std::string SEP = "X";
class Recver : public CallBack
{
    struct DeserializeResult
    {
        std::string data1;
        std::string data2;
    };
public:
    virtual int operator()(Event& ev)
    { 
        std::cout << "fd: " << ev.fd << "正在读取数据" << std::endl;

        int ret = ReadInfo(ev.fd, &ev.inbuffer);
        if (ret <= 0)
        {
            CallBack& Errorer = *ev.errorer;
            Errorer(ev);
            return -1;
        }
        std::cout << "[inbuffer] " << ev.inbuffer << std::endl; 

        while (true)
        {
            std::string info;
            if (SplitString(ev.inbuffer, &info, SEP) == false)
            {
                // write(ev.fd, "?", 2);
                // ev.outbuffer += "?";
                break;
            }

            DeserializeResult dres;
            Deserialize(std::move(info), &dres, SEP);

            std::string res;
            Calc(dres, &res);

            std::string outStr = dres.data1 + "+" + dres.data2;
            outStr += "=" + res;
            outStr += SEP;
            ev.outbuffer += outStr;
        }

        if (!ev.outbuffer.empty())
        {
            ev.R->ModifyEvent(ev.fd, EPOLLIN | EPOLLOUT | EPOLLET);
        }

        return 0;
    }
private:
    int ReadInfo(int fd, std::string* inbuffer)
    {
        while (true)
        {
            char buffer[READ_NUM];
            int sz = read(fd, buffer, READ_NUM - 1);
            if (sz > 0)
            {
                buffer[sz] = 0;
                *inbuffer += buffer;
            }
            else if (sz == 0)
            {
                return 0; // 对端退出
            }
            else
            {
                if (errno == EINTR) continue; // 被信号中断
                if (errno == EAGAIN || errno == EWOULDBLOCK) break; // 底层无数据了

                return -1; // 错误
            }
        }

        return 1; // 正常
    }

    bool Deserialize(const std::string& target, DeserializeResult* dres, const std::string& sep)
    {
        int pos = target.find("+");
        if (pos == std::string::npos)
        {
            return false;
        }

        dres->data1 = target.substr(0, pos);
        dres->data2 = target.substr(pos + 1);
        return true;
    }
    bool Calc(const DeserializeResult& dres, std::string* res)
    {
        int data1 = std::stoi(dres.data1);
        int data2 = std::stoi(dres.data2);

        int sum = data1 + data2;
        *res = std::to_string(sum);
        return true;
    }
};

class Sender : public CallBack
{
public:
    virtual int operator()(Event& ev)
    { 
        int ret = SendInfo(ev.fd, ev.outbuffer);
        if (ret > 0)
        {
            ev.R->ModifyEvent(ev.fd, EPOLLIN | EPOLLET);
        }
        else if (ret == 0)
        {
            // 无事
        }
        else
        {
            CallBack& Errorer = *ev.errorer;
            Errorer(ev);
            return -1;
        }

        return 0; 
    }
private:
    int SendInfo(int fd, std::string& outbuffer)
    {
        while (true)
        {
            int cur = 0;
            const char* start = outbuffer.c_str();
            int len = outbuffer.size();

            int n = write(fd, start + cur, len - cur);
            if (n > 0)
            {
                cur += n;
                if (cur == len)
                {
                    outbuffer.clear();
                    return 1;
                }
            }
            else
            {
                if (errno == EINTR) continue;
                if (errno == EAGAIN || errno == EWOULDBLOCK) 
                {
                    outbuffer.erase(0, cur);
                    return 0;
                }

                return -1;
            }
        }
    }
};

class Errorer : public CallBack
{
public: 
    virtual int operator()(Event& ev)
    { 
        std::cerr << "call errorer" << std::endl;
        ev.R->DeleteEvent(ev.fd);
        close(ev.fd);
        std::cout << "已关闭fd: " << ev.fd << std::endl;
        return 0; 
    }
};