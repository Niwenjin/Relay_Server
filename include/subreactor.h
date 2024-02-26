#ifndef SUBREACTOR_H
#define SUBREACTOR_H
#include "sock_item.h"
#include <unordered_map>

class SubReactor {
  public:
    SubReactor();
    ~SubReactor();

  private:
    int epfd;
    std::unordered_map<int, Sock_item> sock_map;
};
#endif