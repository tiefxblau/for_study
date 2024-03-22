#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main()
{
  int pipefd[2];
  if (pipe(pipefd) == -1)
  {
    printf("pipe error\n");
    exit(1);
  }

  if (fork() == 0)
  {
    close(pipefd[0]);

    const char* msg = "I am you...";
    write(pipefd[1], msg, strlen(msg));

    return 0;
  }
  close(pipefd[1]);

  char buffer[64] = { 0 };
  ssize_t s = read(pipefd[0], buffer, sizeof(buffer) - 1);
  if (s == 0)
  {
    printf("child close\n");
  }
  else if (s > 0)
  {
    buffer[s] = '\0';
    printf("%s\n", buffer);
  }
  else 
  {
    printf("read fail\n");
    exit(1);
  }

  return 0;
}
