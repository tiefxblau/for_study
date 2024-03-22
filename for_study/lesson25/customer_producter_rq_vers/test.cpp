#include "RingQueue.hpp"
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <pthread.h>
#include <unistd.h>

using namespace ns_ring_queue;

void* CustomerRun(void* args)
{
    RingQueue<int>* rq = (RingQueue<int>*)args;

    while (true)
    {
        int num;
        rq->Pop(&num);
        std::cout << pthread_self() << ": get " << num << std::endl;

        // sleep(5);
    }

    return nullptr;
}
void* ProducterRun(void* args)
{
    RingQueue<int>* rq = (RingQueue<int>*)args;

    while (true)
    {
        int num = rand() % 10 - rand() % 10;
        rq->Push(num);
        std::cout << pthread_self() << ": push " << num << std::endl;

        sleep(2);
    } 

    return nullptr;
}

int main()
{
    srand((unsigned int)time(nullptr));
    RingQueue<int>* rq = new RingQueue<int>(10);

    pthread_t customer[2];
    pthread_t producter[2];

    pthread_create(&customer[0], nullptr, CustomerRun, (void*)rq);
    pthread_create(&producter[0], nullptr, ProducterRun, (void*)rq);
    pthread_create(&customer[1], nullptr, CustomerRun, (void*)rq);
    pthread_create(&producter[1], nullptr, ProducterRun, (void*)rq);

    pthread_join(customer[0], nullptr);
    pthread_join(producter[0], nullptr);
    pthread_join(customer[1], nullptr);
    pthread_join(producter[1], nullptr);

    return 0;
}