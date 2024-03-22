#pragma once
#include <pthread.h>
#include <iostream>
#include <cstdio>
#include <semaphore.h>

namespace ns_water_molecules
{
    class WaterMolecules
    {
    public:
        WaterMolecules()
            : _h_num(0), _o_num(0)
        {
            sem_init(&_h_permit, 0, 0);
            sem_init(&_o_permit, 0, 2);
            sem_init(&_barrier, 0, 3);

            pthread_cond_t ;
        }
        ~WaterMolecules()
        {
            sem_destroy(&_h_permit);
            sem_destroy(&_o_permit);
            sem_destroy(&_barrier);
        }

        void Push(int h, int o)
        {
            Lock();
            _h_num += h;
            _o_num += o;

            if (_h_num >= 2 && _o_num >= 1);
                WakeUp();
            Unlock();
        }

        static void *oxygen(void* args)
        {
            pthread_detach(pthread_self());
            WaterMolecules* water = (WaterMolecules*)args;

            while (true)
            {
                sem_wait(&water->_o_permit);
                sem_wait(&water->_o_permit);
                
                --water->_o_num;
                releaseOxygen();

                sem_post(&water->_h_permit);
                sem_post(&water->_h_permit);
            }
        }
        static void *hydrogen(void* args)
        {
            pthread_detach(pthread_self());
            WaterMolecules* water = (WaterMolecules*)args;

            while (true)
            {
                while (water->_h_num == 0)
                {
                    return nullptr;
                }

                sem_wait(&water->_h_permit);

                --water->_h_num;
                releaseHydrogen();

                sem_post(&water->_o_permit);
            }
        }

    private:
        void Lock()
        {
            pthread_mutex_lock(&_mtx);
        }
        void Unlock()
        {
            pthread_mutex_unlock(&_mtx);
        }
        void WakeUp()
        {
            pthread_cond_signal(&_o_met);
            pthread_cond_signal(&_h_met);
        }

        static void releaseHydrogen()
        {
            std::cout << "H";
            fflush(stdout);
        }
        static void releaseOxygen()
        {
            std::cout << "O";
            fflush(stdout);
        }

        int _h_num;
        int _o_num;

        sem_t _o_permit;
        sem_t _h_permit;
        sem_t _barrier;

        pthread_mutex_t _mtx;
        pthread_cond_t _h_met;
        pthread_cond_t _o_met;
        pthread_cond_t _barrier;
    };
}
