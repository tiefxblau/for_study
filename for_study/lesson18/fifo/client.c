#include "comm.h"

int main()
{
  int fd = open("./myfifo", O_WRONLY);
  if (fd < 0)
  {
    perror("open");
    return 1;
  }

  while(1)
  {
    printf("请输入: ");
    fflush(stdout);

    char buffer[64] = {0};

    ssize_t s = read(0, buffer, sizeof(buffer));
    buffer[s - 1] = '\0';

    write(fd, buffer, strlen(buffer));
    
  }

  return 0;
}
