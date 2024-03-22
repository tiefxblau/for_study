#include<iostream>
#include<unistd.h>
using namespace std;
int main()
{
  fork();
  fork() && fork() || fork();
  fork();
  cout << "I'm a proc :)" << endl;
  return 0;
}
