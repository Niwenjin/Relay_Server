#ifndef TCPSERVER_H
#define TCPSERVER_H
#include "reactor.h"

class TCPserver {
  public:
    TCPserver(int port, int thread_num);
    ~TCPserver();
    void run();
    void shutdown();

  private:
    int listenfd;
    int init_socket(int port);
    Reactor *reactor;
};
#endif