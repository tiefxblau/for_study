#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
  int fd1 = open("./log.txt", O_CREAT | O_WRONLY, 0664);
  if (fd1 < 0)
  {
    perror("open");
    exit(1);
  }

  const char* msg = "i like linux!\n";
  write(fd1, msg, strlen(msg));
  close(fd1);

  int fd2 = open("./log.txt", O_RDONLY);
  if (fd2 < 0)
  {
    perror("open");
    exit(1);
  }

  char buffer[1024];
  int s = read(fd2, buffer, sizeof(buffer) - 1);
  buffer[s] = '\0';

  printf("%s\n", buffer);
  close(fd2);

  return 0;
}

