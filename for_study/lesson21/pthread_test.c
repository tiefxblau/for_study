#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

void* pthreadRun(void* arg)
{
    pthread_detach(pthread_self());
    while (1)
    {
        printf("[%s]: ^-^\n", (const char*)arg);
        sleep(3);

        const char* ret = "O-O\n";
        return (void*)ret;
        // pthread_exit((void*)ret);
    }

}

int main()
{
    pthread_t tid = 0;
    pthread_create(&tid, NULL, pthreadRun, (void*)"new_pthread");

    sleep(1);
    void *ret = NULL;
    int n = pthread_join(tid, &ret);

    printf("[main]: %d %s", n, ret);
   // while (1)
   // {
   //     printf("[main]: !!!\n");
   //     sleep(4);
   // }

    return 0;
}
