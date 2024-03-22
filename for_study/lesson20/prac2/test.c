#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main()
{
  if (fork() == 0)
  {
    while (1)
    {
      int* a = NULL;
      a = (int*)66666;
      *a = 0;
      sleep(1);
    }
  }

  int status = 0;
  waitpid(-1, &status, 0);

  printf("exit code: %d, exit signal: %d, core dumped flag: %d\n", (status >> 8) & 0xff, status & 0x7f, (status >> 7) & 1);

  return 0;
}
