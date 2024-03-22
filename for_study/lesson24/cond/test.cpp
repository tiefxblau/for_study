#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <string>

using namespace std;

pthread_cond_t cond;
pthread_mutex_t mtx;

void *Control(void* args)
{
    string name((const char*)args);

    while (true)
    {
        sleep(2);
        cout << name << " command :" << endl;
        pthread_cond_broadcast(&cond);
    }

    return nullptr;
}
void *Work(void* args)
{
    int id = *(int*)args;
    free((int*)args);

    while (true)
    {
        pthread_cond_wait(&cond, &mtx);
        printf("number[%d] is working...\n", id);
    }

    return nullptr;
}

int main()
{
    pthread_t master;
    pthread_t servant[3];

    pthread_cond_init(&cond, nullptr);
    pthread_mutex_init(&mtx, nullptr);

    pthread_create(&master, nullptr, Control, (void*)"master");
    for (int i = 0; i < 3; ++i)
    {
        int *number = new int(i);
        pthread_create(&servant[i], nullptr, Work, number);
    }

    for (int i = 0; i < 3; ++i)
    {
        pthread_join(servant[i], nullptr);
    }
    pthread_join(master, nullptr);

    return 0;
}