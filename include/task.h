#ifndef TASK_H
#define TASK_H
#include "sock_item.h"
#include <functional>

using std::function;

class Task {
  public:
    Task(int epfd, int type, int fd, Sock_item &sock_item);
    void run();

  private:
    int epfd;
    int fd;
    char *buf;
    function<void(int, int, char *)> func;
};
#endif