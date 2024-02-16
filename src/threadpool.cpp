#include "threadpool.h"
#include "sock_item.h"

using std::unique_lock;

ThreadPool::ThreadPool(int n, vector<Sock_item> &sock_items)
    : num(n), stopflag(false), sock_items(sock_items) {
    pool_run();
}

ThreadPool::~ThreadPool() { pool_stop(); }

void ThreadPool::pool_run() {
    for (int i = 0; i < num; i++) {
        threads.emplace_back([this]() {
            while (true) {
                unique_lock<mutex> lock(mutex_queue);

                cond.wait(lock,
                          [this]() { return stopflag || !task_queue.empty(); });

                if (stopflag && task_queue.empty()) {
                    return;
                }

                task_queue.front().run();
                task_queue.pop();
            }
        });
    }
}

void ThreadPool::pool_stop() {
    {
        unique_lock<mutex> lock(mutex_queue);
        stopflag = true;
    }

    cond.notify_all();

    for (thread &thread : threads) {
        thread.detach();
    }
}

void ThreadPool::add_task(int epfd, int type, int fd) {
    unique_lock<mutex> lock(mutex_queue);
    task_queue.emplace(Task(epfd, type, fd, sock_items[fd]));
    cond.notify_one();
}