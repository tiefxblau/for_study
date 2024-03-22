#include <stdio.h>
#include <signal.h>

volatile int flag = 1;

void handler(int signo)
{
  flag = 0;
  printf("receive %d\n", signo);
}

int main()
{
  signal(2, handler);

  while (flag);

  printf("normally exit\n");

  return 0;
}
