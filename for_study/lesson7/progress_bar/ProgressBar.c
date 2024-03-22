#include <stdio.h>
#include <unistd.h>
#define NUM 100
int main()
{
  char bar[NUM + 1] = {0};
  char sym[] = "|\\-/";
  for (int i = 0; i <= NUM; ++i)
  {
    printf("[%-100s] %-3d%% %c\r", bar, i, sym[i % 4]);
    fflush(stdout);
    bar[i] = '=';
    if (i < NUM)
      bar[i + 1] = '>';
    usleep(1000 * 50);
  }
  printf("\n");
  return 0;
}
