#include "comm.h"

int main()
{
  key_t key = ftok(PATH_NAME, PROJ_ID);
  if (key < 0)
  {
    perror("ftok");
    return 1;
  }

  printf("%d\n", key);

  int shmid = shmget(key, SIZE, IPC_CREAT | IPC_EXCL | 0666);
  if (shmid < 0)
  {
    perror("shmget");
    return 2;
  }
  
  printf("%d\n", shmid);

  char* shm = (char*)shmat(shmid, NULL, 0);
  if (shm == (char*)-1)
  {
    perror("shmat");
    return 3;
  }
  shm[0] = 0;

  int time = 100;
  while(time--)
  {
    printf("%s\n", shm);
    sleep(2);
  }

  shmdt(shm);

  shmctl(shmid, IPC_RMID, NULL);

  return 0;
}
