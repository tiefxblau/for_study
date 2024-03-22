#include <iostream>
#include <pthread.h>
#include <string>

using namespace std;

pthread_mutex_t mtx;
pthread_cond_t cond;

void* Print(void* args)
{
    string name((const char*)args);

    while (true)
    {
        pthread_cond_wait(&cond, &mtx);
        cout << "I'm " << name << endl;
    }

    return nullptr;
}
void* Control(void* args)
{
    while (true)
    {
        pthread_cond_signal(&cond);
    }

    return nullptr;
}

int main()
{
    pthread_t threadA;
    pthread_t threadB;
    pthread_t master;

    pthread_create(&threadA, nullptr, Print, (void*)"threadA");
    pthread_create(&threadB, nullptr, Print, (void*)"threadB");
    pthread_create(&master, nullptr, Control, (void*)"master");

    pthread_mutex_init(&mtx, nullptr);
    pthread_cond_init(&cond, nullptr);

    pthread_join(threadA, nullptr);
    pthread_join(threadB, nullptr);
    pthread_join(master, nullptr);

    return 0;
}