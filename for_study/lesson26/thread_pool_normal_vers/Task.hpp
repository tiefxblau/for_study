#pragma once
#include <cassert>
#include <cstdio>
#include <pthread.h>

namespace ns_task
{
    class Task
    {
    public:
        void operator()()
        {
            printf("[%lx]: task finished: %d\n", pthread_self(), Calc());
        }

    private:
        int _left;
        int _right;
        char _op;

    public:
        Task() {}
        Task(int left, int right, char op)
            : _left(left), _right(right), _op(op) {}

        int Calc()
        {
            int ret = 0;
            switch (_op)
            {
            case '+':
                ret = _left + _right;
                break;
            case '-':
                ret = _left - _right;
                break;
            case '*':
                ret = _left * _right;
                break;
            case '/':
                ret = _left / _right;
                break;
            case '%':
                ret = _left % _right;
                break;
            default:
                assert(false);
                break;
            }

            return ret;
        }

        ~Task() {}
    };
} // namespace ns_task
