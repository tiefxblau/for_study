#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
  extern char** environ;
  printf("myexec!\n");
  int i = 0;
  for (; environ[i]; ++i)
  {
    printf("%s\n", environ[i]);
  }

  return 0;
}
