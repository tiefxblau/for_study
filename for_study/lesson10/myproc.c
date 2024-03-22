#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
  printf("%d\n", argc);
  if (argc != 2)
  {
    printf("%s: using -x|-o\n", argv[0]);
    return 1;
  }
  if (strcmp(argv[1], "-x") == 0)
  {
    printf("xxxxxxxx\n");
  }
  else if (strcmp(argv[1], "-o") == 0)
  {
    printf("oooooooo\n");
  }
  return 0;
}
