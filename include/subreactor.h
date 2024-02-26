#ifndef SUBREACTOR_H
#define SUBREACTOR_H
#include "sock_item.h"
#include <unordered_map>

class SubReactor {
  public:
    SubReactor();
    ~SubReactor();
    void add_event(int fd);
    void run();

  private:
    int epfd;

    int epoll_init();
    void relay_loop();
    void relay_event(int fd);
    
    std::unordered_map<int, Sock_item> sock_map;
};
#endif