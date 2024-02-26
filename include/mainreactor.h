#ifndef MAINREACTOR_H
#define MAINREACTOR_H
#include "subreactor.h"
#include <thread>
#include <vector>

class MainReactor {
  public:
    MainReactor(int n);
    ~MainReactor();
    void run();

  private:
    int thread_num;
    int epfd;
    int listenfd;
    
    std::vector<std::thread> threads;
    std::vector<SubReactor> subreactors;

    int epoll_init();
    int thread_init();
    int listensock_init();
    void epoll_add_listen();

    void accept_loop();
    void dispatch(int fd);
};

#endif