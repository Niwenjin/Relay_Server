#ifndef TASK_H
#define TASK_H
#include "sock_item.h"
#include <functional>

class Task {
  public:
    Task(int epfd, int type, int fd, Sock_item &sock_item)
        : epfd(epfd), fd(fd), buf(sock_item.buf) {
          // 创建任务函数
        }
    ~Task() {}
    void run() { func(epfd, fd, buf); };

  private:
    int epfd;
    int fd;
    char *buf;
    std::function<void(int, int, char *)> func;
};
#endif