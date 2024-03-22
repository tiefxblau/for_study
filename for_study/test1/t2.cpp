#include <iostream>
#include <string>

using namespace std;

int main()
{
    string s = "hello world";
    cout << s.find("world", 100) << endl;
    cout << string::npos << endl;

    return 0;
}