#ifndef TASK_H
#define TASK_H
#include <functional>

using std::function;

class Task {
  public:
    Task(int epfd, int type, int fd, char *buf);
    void run();

  private:
    int epfd;
    int fd;
    char *buf;
    function<void(int, int, char *)> func;
};
#endif