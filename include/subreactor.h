#ifndef SUBREACTOR_H
#define SUBREACTOR_H
#include "sock_item.h"
#include <unordered_map>

class SubReactor {
  public:
    SubReactor();
    ~SubReactor();
    void add_relay_pair(int fd1, int fd2);
    void run();

  private:
    int epfd;

    int epoll_init();
    void relay_loop();
    void relay_event(int fd);
    void epoll_add(int fd);

    std::unordered_map<int, Sock_item> sock_map;
    std::unordered_map<int, int> relay_map;
};
#endif