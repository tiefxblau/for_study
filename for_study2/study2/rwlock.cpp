#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>

#include <assert.h>
#include <string.h>

#include <iostream>
#include <vector>
#include <string>
using namespace std;

void Pthread_create(pthread_t& thread, void* (*work)(void*), void* arg)
{
    if (pthread_create(&thread, nullptr, work, arg) != 0)
    {
        assert(false);
    }
}

void* Pthread_join(pthread_t thread)
{
    void* ret = nullptr;
    if (pthread_join(thread, &ret) != 0)
    {
        assert(false);
    }
    return ret;
}

struct RW_lock
{
    sem_t rcMutex;
    sem_t wcMutex;

    uint readerCount;

    sem_t writeMutex;


    RW_lock()
        : readerCount(0)
    {
        sem_init(&rcMutex, 0, 1);
        sem_init(&wcMutex, 0, 1);
        sem_init(&writeMutex, 0, 1);
    }
};

struct Message
{
    string content;
};

char buffer[256];
RW_lock rwlock;

void ReadAndSay()
{
    cout << pthread_self() << ": read " << buffer << endl;
}

void* Read(void* arg)
{
    sem_wait(&rwlock.wcMutex);
    sem_wait(&rwlock.rcMutex);
    
    if (rwlock.readerCount == 0)
    {
        sem_wait(&rwlock.writeMutex);
    }
    ++rwlock.readerCount;

    sem_post(&rwlock.rcMutex);
    sem_post(&rwlock.wcMutex);

    ReadAndSay();

    sem_wait(&rwlock.rcMutex);

    --rwlock.readerCount;
    if (rwlock.readerCount == 0)
    {
        sem_post(&rwlock.writeMutex);
    }

    sem_post(&rwlock.rcMutex);

    sleep(1);
}

void WriteAndSay(const string& content)
{
    strcpy(buffer, content.c_str());
    cout << pthread_self() << ": write " << content << endl;
}

void* Write(void* arg)
{
    string s = ((Message*)arg)->content + to_string(rand());

    sem_wait(&rwlock.wcMutex);

    sem_wait(&rwlock.writeMutex);
    WriteAndSay(s);
    sem_post(&rwlock.writeMutex);

    sem_post(&rwlock.wcMutex);

}

int main()
{
    srand(time(nullptr));
    constexpr int kReadersNum = 3;
    constexpr int kWritersNum = 1;
    vector<pthread_t> readers(kReadersNum);
    vector<pthread_t> writers(kWritersNum);

    for (auto& reader : readers)
    {
        Pthread_create(reader, Read, nullptr);
    }
    // cout << "a" << endl;

    int i = 0;
    Message* msg = new Message;
    msg->content = "hello ";
    for (auto& writer : writers)
    {
        // cout << s << endl;
        Pthread_create(writer, Write, &msg);
        ++i;
    }

    // cout << "b" << endl;
    for (auto reader : readers)
    {
        Pthread_join(reader);
    }

    // cout << "c" << endl;
    for (auto writer : writers)
    {
        Pthread_join(writer);
    }

    return 0;
}
