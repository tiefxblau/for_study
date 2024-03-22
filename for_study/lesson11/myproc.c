#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
  pid_t id = fork();

  if (id == 0)
  {
    int cnt = 15;
    while (cnt--)
    {
      printf("[%d]: I an a child process\n", getpid());
      sleep(1);
    }

    return 4;
  }

  int status = 0;
  pid_t ret = waitpid(id, &status, 0);

  if (ret > 0)
  {
    if (WIFEXITED(status))
    {
      printf("%d %d %d\n", WEXITSTATUS(status), (status >> 7) & 1, (status) & 0x7f);
    }
    else
    {
      printf("%d %d %d\n", WEXITSTATUS(status), (status >> 7) & 1, (status) & 0x7f);
    }
  }
  return 0;
}
