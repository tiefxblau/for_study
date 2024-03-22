#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <cerrno>

using namespace std;

bool SetNoBlock(int fd)
{
    int fl = fcntl(fd, F_GETFL);
    if (fl < 0)
        return false;
    else
    {
        fcntl(fd, F_SETFL, fl | O_NONBLOCK);
        return true;
    }
}

int main()
{
    SetNoBlock(0);
    while (true)
    {
        char buffer[1024] = { 0 };
        int sz = read(0, buffer, sizeof(buffer) - 1);
        if (sz > 0)
        {
            buffer[sz] = 0;
            cout << "read success, errno: " << errno << endl;
            cout << "lenth: " << sz << endl;
        }
        else
        {
            if (errno != EAGAIN)
                cout << "read failed, errno: " << errno << endl;
            else
                cout << "read none, errno: " << errno << endl;
        }

        sleep(2);
    }

    return 0;
}