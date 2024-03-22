#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
  FILE* fp1 = fopen("./log.txt", "wb");
  if (!fp1)
  {
    perror("fopen");
    exit(1);
  }

  const char* msg = "linux so easy!\n";
  fwrite(msg, strlen(msg) + 1, 1, fp1);
  fclose(fp1);

  FILE* fp2 = fopen("./log.txt", "rb");
  if (!fp2)
  {
    perror("fopen");
    exit(1);
  }

  char buffer[1024];
  fread(buffer, sizeof(buffer), 1, fp2);

  printf("%s\n", buffer);
  fclose(fp2);

  return 0;
}
