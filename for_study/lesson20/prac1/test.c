#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void header(int signo)
{
  printf("signal: %d\n", signo);
  exit(1);
}

int main()
{
  for (int i = 1; i < 32; ++i)
  {
    signal(i, header);
  }

  while (1)
  {
    printf("阿巴阿巴\n");
    sleep(3);
  }

  return 0;
}
