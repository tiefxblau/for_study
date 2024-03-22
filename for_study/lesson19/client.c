#include "comm.h"

int main()
{
  key_t key = ftok(PATH_NAME, PROJ_ID);
  if (key < 0)
  {
    perror("ftok");
    return 1;
  }

  int shmid = shmget(key, SIZE, IPC_CREAT | 0666);
  if (shmid < 0)
  {
    perror("shmget");
    return 2;
  }

  char* shm = (char*)shmat(shmid, NULL, 0);
  
  int cnt = 0;
  while (1)
  {
    shm[cnt] = 'A' + cnt;
    shm[cnt + 1] = 0;
    ++cnt;
    sleep(2);
  }

  shmdt(shm);

  return 0;
}
