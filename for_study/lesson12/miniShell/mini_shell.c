#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#define MAX_LEN 128
#define MAX_NUM 64

int main()
{
  char _cmd[MAX_LEN] = { 0 };
  char *cmd[MAX_NUM] = { 0 };

  while (1)
  {
    printf("[tiefxblau@Hostname mydir]$ ");
    fflush(stdout);

    fgets(_cmd, MAX_LEN, stdin);
    _cmd[strlen(_cmd) - 1] = 0;

    const char* sep= " ";
    cmd[0] = strtok(_cmd, sep);
    int i = 1;
    while (cmd[i] = strtok(NULL, sep))
    {
      ++i;
    }

    if (strcmp(cmd[0], "cd") == 0)
    {
      chdir(cmd[1]);
      continue;
    }

    if (fork() == 0)
    {
      execvp(cmd[0], cmd);
      exit(1);
    }

    int status = 0;
    waitpid(-1, &status, 0);
    printf("exit_code:%d\n", (status >> 8) & 0xff);
  }

  return 0;
}
