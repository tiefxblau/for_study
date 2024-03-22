#include "./mysys.h"
#include <iostream>
#include <algorithm>
#include <string.h>
using namespace std;

void Usage()
{
    cerr << "usage:/n/tmytile -n <pathname>" << endl;
}

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        Usage();
        exit(-1);
    }

    string path = argv[2];
    string para = argv[1];
    uint n = stoul(para.substr(1));
    if (n <= 0)
    {
        Usage();
        exit(-1);
    }

    struct stat buf;
    if (stat(path.c_str(), &buf) != 0)
    {
        cerr << "error :(" << endl;
        exit(-2);
    }

    if (S_ISDIR(buf.st_mode))
    {
        cerr << "mytail: error reading '" << path << "' : Is a directory" << endl;
    }

    int fd = open(path.c_str(), O_RDONLY);

    char* ptr = (char*)mmap(nullptr, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0);

    int i = buf.st_size - 1;
    for (; i >= 0; --i)
    {
        if (ptr[i] == '\n')
        {
            if (--n == 0)
                break;
        }
    }
    ++i;

    lseek(fd, i, SEEK_SET);
    char buffer[1024];
    int sz = 0;
    while ((sz = read(fd, buffer, 1023)) > 0)
    {
        buffer[sz] = 0;
        cout << buffer;
    }
    cout << endl;

    return 0;
}