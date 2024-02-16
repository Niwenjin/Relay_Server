#ifndef THREADPOOL_H
#define THREADPOOL_H
#include "sock_item.h"
#include "task.h"
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <unordered_map>
#include <vector>

#define BUFFER_SIZE 128
#define MAX_CONN_SIZE 65536

using std::condition_variable;
using std::function;
using std::mutex;
using std::queue;
using std::thread;
using std::unordered_map;
using std::vector;

class ThreadPool {
  public:
    ThreadPool(int n, vector<Sock_item> &sock_items);
    ~ThreadPool();

    void add_task(int epfd, int type, int fd);

  private:
    int num;
    vector<thread> threads;
    queue<Task> task_queue;
    mutex mutex_queue;
    condition_variable cond;

    void pool_run();
    void pool_stop();
    bool stopflag;

    // char buf[MAX_CONN_SIZE][BUFFER_SIZE];
    vector<Sock_item> &sock_items;
};

#endif