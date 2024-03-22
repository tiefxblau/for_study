#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main()
{
  char *const env[] = {
    "ENV=sleep",
    "QWER=????????",
    NULL
  };
  printf("start:\n");

  if (fork() == 0)
  {
    execle("./myexec", "myexec", NULL, env);
		exit(1);
  }

	pid_t ret = waitpid(-1, NULL, 0);
	if (ret > 0)
  {
    printf("wait successfully\n");
  }
  return 0;
}
