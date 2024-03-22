#pragma once
#include <pthread.h>
#include <queue>

namespace ns_thread_pool
{
    const int default_num = 3;

    template <class T>
    class ThreadPool
    {
    private:
        std::queue<T> _tq;
        int _num;

        pthread_mutex_t _mtx;
        pthread_cond_t _cond;

        static ThreadPool* _ins;

        ThreadPool(int n = default_num): _num(n)
        {
            pthread_mutex_init(&_mtx, nullptr);
            pthread_cond_init(&_cond, nullptr);

            InitThread();
        }
    public:
        ThreadPool(ThreadPool& tp) = delete;
        ThreadPool& operator=(ThreadPool& tp) = delete;

        static ThreadPool* GetInstance()
        {
            static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
            if (_ins == nullptr)
            {
                pthread_mutex_lock(&mtx);
                if (_ins == nullptr)
                {
                    _ins = new ThreadPool;
                }
                pthread_mutex_unlock(&mtx);
            }

            return _ins;
        }

        void InitThread()
        {
            pthread_t tid;
            for (int i = 0; i < _num; ++i)
            {
                pthread_create(&tid, nullptr, ThreadRun, (void*)this);
            }
        }
        void Push(const T& task)
        {
            Lock();

            _tq.push(task);

            Unlock();
            WakeUp();
        }
        bool isEmpty()
        {
            return _tq.empty();
        }
    private:
        void WakeUp()
        {
            pthread_cond_signal(&_cond);
        }
        void Lock()
        {
            pthread_mutex_lock(&_mtx);
        }
        void Unlock()
        {
            pthread_mutex_unlock(&_mtx);
        }
        void Wait()
        {
            pthread_cond_wait(&_cond, &_mtx);
        }
        void Pop(T* out)
        {
            Lock();

            while (isEmpty())
            {
                Wait();
            }
            *out = _tq.front();
            _tq.pop();

            Unlock();
        }

        static void* ThreadRun(void* args)
        {
            pthread_detach(pthread_self());
            ThreadPool<T>* tp = (ThreadPool<T>*)args;

            while (true)
            {
                T task;
                tp->Pop(&task);

                task();
            }

            return nullptr;
        }
    };

    template <class T>
    ThreadPool<T>* ThreadPool<T>::_ins = nullptr;
}
