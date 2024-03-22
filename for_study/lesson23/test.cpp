#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <cstdio>

using namespace std;

class Ticket
{
public:
    Ticket()
    :_ticketNum(2500)
    {
        pthread_mutex_init(&_mtx, nullptr);
    }

    bool GetTicket()
    {
        static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
        pthread_mutex_lock(&mtx);

        bool ret = true;
        if (_ticketNum > 0)
        {
            usleep(1000);

            printf("thread[%lx]: get ticket(%d)\n", pthread_self(), _ticketNum);
            --_ticketNum;
        }
        else
        {
            printf("nothing...\n");
            ret = false;
        }

        pthread_mutex_unlock(&mtx);
        return ret;
    }

    ~Ticket()
    {
        pthread_mutex_destroy(&_mtx);
    }
private:
    int _ticketNum;
    pthread_mutex_t _mtx;
};

Ticket ticket;

void* ThreadRun(void* arg)
{
    bool flag = true;
    while (flag)
    {
        flag = ticket.GetTicket();
    }

    return nullptr;
}

int main()
{
    pthread_t tid[5];
    for (int i = 0; i < 5; ++i)
    {
        int *id = new int(i);
        pthread_create(&tid[i], nullptr, ThreadRun, id);
    }

    for (int i = 0; i < 5; ++i)
    {
        pthread_join(tid[i], nullptr);
    }

    return 0;
}
