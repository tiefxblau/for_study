#include <iostream>

int SumToTop(int n)
{
  int ret = 0;
  for (int i = 1; i <= n; ++i)
  {
    ret += i;
  }
  return ret;
}

int main()
{
  int n = 100;
  int b = SumToTop(n);
  std::cout << b << std::endl;
  return 0;
}
