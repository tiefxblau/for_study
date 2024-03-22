#include "./mysys.h"
#include <iostream>
#include <string.h>
#include <string>
#include <algorithm>
using namespace std;

void Usage()
{
    cerr << "Usage:\n\tmyls [-l] <pathname>" << endl;
}

void outputFolder(const string& pathname, bool l)
{
    DIR* dp = opendir(pathname.c_str());
    if (dp == nullptr)
    {
        cerr << "error :(" << endl;
        exit(-1);
    }

    struct dirent* d;
    if (l)
    {
        while ((d = readdir(dp)) != nullptr)
        {
            struct stat state;
            string filename = pathname + "/" + d->d_name;
            stat(filename.c_str(), &state);
            printf("%d %d %d %d %d %d %s\n", state.st_mode, state.st_nlink, state.st_uid, 
                state.st_gid, state.st_size, state.st_atim.tv_sec, d->d_name);
        }
    }
    else
    {
        while ((d = readdir(dp)) != nullptr)
        {
            printf("%s ", d->d_name);
        }
        cout << endl;
    }
}

int main(int argc, char* argv[])
{
    if (argc > 3)
    {
        Usage();
        return -1;
    }

    string dir;
    bool l = false;

    if (argc == 1)
    {
        dir = ".";
    }
    else if (argc == 2)
    {
        dir = strcmp(argv[1], "-l") == 0 ? "." : argv[1];
        l = strcmp(argv[1], "-l") == 0;
    }
    else if (argc == 3)
    {
        l = true;
        if (strcmp(argv[1], "-l") == 0)
            swap(argv[1], argv[2]);

        if (strcmp(argv[2], "-l") == 0)
        {
            dir = argv[1];
        }
        else
        {
            Usage();
            return -1;
        }
    }
    
    outputFolder(dir, l);
    return 0;
}