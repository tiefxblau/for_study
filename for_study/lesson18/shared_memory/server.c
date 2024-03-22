#include "comm.h"

int main()
{
  key_t key = ftok(PATH_NAME, PROJ_ID);
  printf("%d\n", key);
  if (key < 0)
  {
    perror("ftok");
    return 1;
  }

  int shmid = shmget(key, sizeof(1024 * 8), IPC_CREAT | IPC_EXCL);
  if (shmid < 0)
  {
    perror("shmget");
    return 2 ;
  }
  
  shmctl(shmid, IPC_RMID, NULL);

  return 0;
}
