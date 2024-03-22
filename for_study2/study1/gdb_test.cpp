#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include <unistd.h>
using namespace std;

void print()
{
    cout << "hello" << endl;
    cout << "my name is print()" << " :)" << endl;
}

class Widget
{
private:
    int size_ = 0;
    string name_;
    bool xy_ = false;

public:
    int size() const { return size_; }
    string getName() const { return name_; }

    void setSize(int sz) { size_ = sz; }
    void setName(const string& name) { name_ = name; }

    Widget(int sz, const string& name)
        : size_(sz)
        , name_(name) {}

    Widget() = default;
};

void ThreadRun(int arg)
{
    cout << "I'm " << this_thread::get_id() << " zzz..." <<endl;
    sleep(5);
    cout << "oh, arg: " << arg << endl;
}

mutex ma;
mutex mb;

void DeadMutex(bool a)
{
    if (a)
    {
        ma.lock();
        sleep(1);
        mb.lock();

        mb.unlock();
        ma.unlock();
    }
    else
    {
        mb.lock();
        sleep(1);
        ma.lock();

        ma.unlock();
        mb.unlock();
    }
}

int main()
{
    int a = 0;

    thread t1 = thread(DeadMutex, true);
    thread t2 = thread(DeadMutex, false);
    thread t3 = thread(ThreadRun, 3);
    thread t4 = thread(ThreadRun, 4);

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    Widget widget(10, "disappear");
    print();
    widget.setSize(20);
    cout << widget.getName() << endl;
    widget.setName("noway");
    for (int i = 0; i < 100; ++i)
    {
        a += 2 * i + 1; 
    }
    cout << "finish" << endl;

    return 0;
}
