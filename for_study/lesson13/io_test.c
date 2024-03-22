#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
  int fd = open("./log.txt", O_RDWR | O_TRUNC);
  if (fd < 0)
  {
    perror("open");
  }

  dup2(fd, 1);

  const char* msg = "dame!!!\n";
  fprintf(stdout, "%s", msg);

  return 0;
}
