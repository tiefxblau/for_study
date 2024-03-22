#pragma once
#include <vector>
#include <semaphore.h>
#include <pthread.h>

namespace ns_ring_queue
{
    template <class T>
    class RingQueue
    {
    public:
        RingQueue(int n)
            : _c_step(0), _p_step(0), _cap(n), _rq(n)
        {
            sem_init(&_sem_blank, 0, n);
            sem_init(&_sem_data, 0, 0);

            pthread_mutex_init(&_p_mtx, nullptr);
            pthread_mutex_init(&_c_mtx, nullptr);
        }

        void Push(const T& in)
        {
            sem_wait(&_sem_blank);

            pthread_mutex_lock(&_p_mtx);
            _rq[_p_step] = in;
            ++_p_step;
            _p_step %= _cap;
            pthread_mutex_unlock(&_p_mtx);

            sem_post(&_sem_data);
        }
        void Pop(T* out)
        {
            sem_wait(&_sem_data);

            pthread_mutex_lock(&_c_mtx);
            *out = _rq[_c_step];
            ++_c_step;
            _c_step %= _cap;
            pthread_mutex_unlock(&_c_mtx);

            sem_post(&_sem_blank);
        }

        ~RingQueue()
        {
            sem_destroy(&_sem_blank);
            sem_destroy(&_sem_data);

            pthread_mutex_destroy(&_p_mtx);
            pthread_mutex_destroy(&_c_mtx);
        }
    private:
        std::vector<T> _rq;
        int _cap;

        sem_t _sem_blank;
        sem_t _sem_data;
        int _c_step;
        int _p_step;

        pthread_mutex_t _p_mtx;
        pthread_mutex_t _c_mtx;
    };
} // namespace ns_ring_queue
