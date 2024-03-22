#include "./mysys.h"
#include <iostream>
using namespace std;

void Usage()
{
    cerr << "usage:/n/tStat <pathname>" << endl;
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        Usage();
        return -1;
    }

    struct stat buf;
    if (stat(argv[1], &buf) != 0)
    {
        cerr << "error" << endl;
        return -2;
    }

    cout << "SIZE: " << buf.st_size << endl;
    cout << "BLOCKS: " << buf.st_blocks << endl;
    cout << "NLINK: " << buf.st_nlink << endl;

    return 0;
}