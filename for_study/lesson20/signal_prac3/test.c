#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

void sigprint(sigset_t *set);
void sigcb(int signo)
{
  sigset_t set;
  sigemptyset(&set);
  while (1)
  {

    sigpending(&set);
    sigprint(&set);
    sleep(1);
  }
  printf("signal: %d\n", signo);
 // exit(1);
}
void sigprint(sigset_t *set)
{
  int i = 1;
  for (;i < 32; ++i)
  {
    if (sigismember(set, i))
    {
      printf("1");
    }
    else 
    {
      printf("0");
    }
  }
    printf("\n");
}

int main()
{
  sigset_t set;
  
  int i = 0;
  for (i = 1; i < 32; ++i)
  {
    signal(i, sigcb);
  }

  sigemptyset(&set);
  sigaddset(&set, 2);
  sigaddset(&set, 40);

  //sigprocmask(SIG_SETMASK, &set, NULL);

   
  //struct sigaction action;
  //memset(&action, 0, sizeof(action));
  
  //action.sa_handler = sigcb;
  
  //signal(3, sigcb);

  //sigaction(3, &action, 0);
  while (1)
  {
    printf("镜中假象...\n");
    sigpending(&set);
    sigprint(&set);
    sleep(1);
  }

  return 0;
}
