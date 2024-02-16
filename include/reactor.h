#ifndef REACTOR_H
#define REACTOR_H
#include "threadpool.h"
#include <sys/epoll.h>
#include <unordered_map>
#include <vector>
#include "sock_item.h"

using std::unordered_map;
using std::vector;

class Reactor {
  public:
    Reactor(int fd, int n);
    ~Reactor();
    int reactor_loop();

  private:
    int epfd;
    int listenfd;
    int init_reactor();
    void init_buffer();

    ThreadPool *pool;

    vector<Sock_item> sock_items;

    int set_event(int fd, int event);
    int del_event(int fd);

    int event_accept(int fd);
    int event_read(int fd);
    int event_write(int fd);
};
#endif