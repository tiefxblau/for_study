#include "comm.h"

int main()
{
  umask(0);

  if (mkfifo("./myfifo", 0666) < 0)
  {
    perror("mkfifo");
    return 1;
  }
  
  int fd = open("./myfifo", O_RDONLY);
  if (fd < 0)
  {
    perror("open");
    return 2;
  }
  
  while (1)
  {
    char buffer[64] = {0};
    
    ssize_t s = read(fd, buffer, sizeof(buffer) - 1);
    if (s == 0)
    {
      printf("client quit ...\n");
      break;
    }
    else if (s > 0)
    {
      buffer[s] = '\0';
      if (strcmp(buffer, "kill") == 0)
      {
        printf("you died...\n");
      }
      else if (strcmp(buffer, "show") == 0)
      {
        if (fork() == 0)
        {
          execl("/usr/bin/ls", "ls", "-l", NULL);
        }

        waitpid(-1, NULL, 0);
      }
      else 
      {
        printf("client# %s\n", buffer);
      } 
    }
    else 
    {
      perror("read");
      return 3;
    }
  }
  return 0;
}
