#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main()
{
  pid_t id = fork();

  if (id == 0)
  {
    while (1)
    {
      printf("hhh...\n");
      sleep(2);
    }
  }
  else 
  {
    while (1)
    {
      printf("parent...\n");
      sleep(2);
    }
  }
  return 0;
}
