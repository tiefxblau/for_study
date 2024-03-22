#include "Task.hpp"
#include <iostream>
#include "thread_pool_singleton.hpp"
#include <ctime>
#include <cstdlib>
#include <unistd.h>

using namespace ns_task;
using namespace ns_thread_pool;

int main()
{
    std::cout << "processs is doing other thing" << std::endl;
    sleep(3);

    srand((unsigned int)time(nullptr));
    ThreadPool<Task>* tp = ThreadPool<Task>::GetInstance();

    while (true)
    {
        int left = rand() % 20 + 1;
        int right = rand() % 20 + 1;
        char op = "+-*/%"[rand() % 5];
        Task t(left, right, op);
        tp->Push(t);
        std::cout << "task: " << left << " " << op << " " << right << std::endl;

        sleep(1);
    }

    return 0;
}