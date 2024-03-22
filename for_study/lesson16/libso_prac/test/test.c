#include <stdio.h>
#include "add.h"
#include "sub.h"

int main()
{
  int x = 10, y = -20;
  printf("%d %d\n", Add(x, y), Sub(x, y));

  return 0;
}
