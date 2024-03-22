#pragma once
#include <iostream>
#include <pthread.h>
#include <queue>

namespace ns_cp
{
    const int default_cap = 5;

    template <class T>
    class BlockQueue
    {
    public:
        BlockQueue(int cap = default_cap)
            : _cap(cap)
        {
            pthread_mutex_init(&_mtx, nullptr);
            pthread_cond_init(&_is_full, nullptr);
            pthread_cond_init(&_is_empty, nullptr);
        }

        void Push(const T &in) //生产函数
        {
            LockQueue();

            while (isFull())
            {
                ProducterWait();
            }

            _bq.push(in);

            UnlockQueue();
            if (_bq.size() > _cap / 2)
            {
                WakeUpCustomer();
            }
        }
        void Pop(T *out)    //消费函数 
        {
            LockQueue();

            while (isEmpty())
            {
                CustomerWait();
            }

            *out = _bq.front();
            _bq.pop();

            UnlockQueue();
            if (_bq.size() < _cap / 2)
            {
                WakeUpProducter();
            }
        }

        ~BlockQueue()
        {
            pthread_mutex_destroy(&_mtx);
            pthread_cond_destroy(&_is_full);
            pthread_cond_destroy(&_is_empty);
        }

    private:
        bool isFull()
        {
            return _cap == _bq.size();
        }
        bool isEmpty()
        {
            return _bq.empty();
        }

        void ProducterWait()
        {
            pthread_cond_wait(&_is_empty, &_mtx);
        }
        void CustomerWait()
        {
            pthread_cond_wait(&_is_full, &_mtx);
        }

        void WakeUpProducter()
        {
            pthread_cond_signal(&_is_empty);
        }
        void WakeUpCustomer()
        {
            pthread_cond_signal(&_is_full);
        }

        void LockQueue()
        {
            pthread_mutex_lock(&_mtx);
        }
        void UnlockQueue()
        {
            pthread_mutex_unlock(&_mtx);
        }

    private:
        std::queue<T> _bq;
        int _cap;

        pthread_mutex_t _mtx;     //锁
        pthread_cond_t _is_full;  //判断队列是否为满， 消费者对此等待
        pthread_cond_t _is_empty; //判断队列是否为空， 生产者对此等待
    };
}