#pragma once
#include <fcntl.h>
#include <iostream>

bool SetNoBlock(int fd)
{
    int fl = fcntl(fd, F_GETFL);
    if (fl < 0)
    {
        std::cerr << "fcntl getfl error" << std::endl;
        return false;
    }

    if (fcntl(fd, F_SETFL, fl | O_NONBLOCK) < 0)
    {
        std::cerr << "fcntl setfl error" << std::endl;
        return false;
    }

    return true;
}
bool SplitString(std::string& string, std::string* splitStr, const std::string& sep)
{
    int pos = string.find(sep);
    if (pos == std::string::npos)
    {
        return false;
    }

    *splitStr = string.substr(0, pos);
    pos += sep.size();
    string.erase(0, pos);
    return true;
}