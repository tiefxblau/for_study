#include "BlockQueue.hpp"
#include "Task.hpp"
#include <ctime>
#include <unistd.h>
#include <cstdlib>
#include <cstdio>

using namespace ns_cp;
using namespace ns_task;

void* ProducterRun(void* args)
{
    BlockQueue<Task>* bq = (BlockQueue<Task>*)args;

    while (true)
    {
        int left = rand() % 20 + 1;
        int right = rand() % 20 + 1;
        char ops[5] = { '+', '-', '*', '/', '%'};
        char op = ops[rand() % 5];

        Task in(left, right, op);
        bq->Push(in);

        std::cout << "producter: task: " << left << " " << op << " " << right << " = ?" << std::endl;
        // sleep(1);
    }

    return nullptr;
}
void* CustomerRun(void* args)
{
    BlockQueue<Task>* bq = (BlockQueue<Task>*)args;

    while (true)
    {
        Task out;
        bq->Pop(&out);

        out();
        sleep(2);
    }
}

int main()
{
    srand((unsigned int)time(nullptr));

    BlockQueue<Task>* bq = new BlockQueue<Task>();

    pthread_t customer[3];
    pthread_t producter;

    for (int i = 0; i < 3; ++i)
        pthread_create(&customer[i], nullptr, CustomerRun, bq);
    pthread_create(&producter, nullptr, ProducterRun, bq);

    pthread_join(producter, nullptr);
    for (int i = 0; i < 3; ++i)
        pthread_join(customer[i], nullptr);

    return 0;
}