#ifndef TCPSERVER_H
#define TCPSERVER_H
#include "reactor.h"

class TCPserver {
  public:
    TCPserver(int port);
    ~TCPserver();
    void run();
    void shutdown();

  private:
    int listenfd;
    int init_socket(int port);
    Reactor *reactor;
};
#endif