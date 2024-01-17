#ifndef REACTOR_H
#define REACTOR_H
#include "sock_item.h"
#include <sys/epoll.h>
#include <unordered_map>

using std::unordered_map;

class Reactor {
  public:
    Reactor(int fd);
    ~Reactor();
    int reactor_loop();

  private:
    int epfd;
    int listenfd;
    unordered_map<int, Sock_item *> sock_items;
    int init_reactor();

    int set_event(int fd, int event);
    int del_event(int fd);

    int event_accept(int fd);
    int event_read(int fd);
    int event_write(int fd);
};
#endif