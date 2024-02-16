#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include "message.h"
#include <vector>
#include <netinet/in.h>

using std::vector;

class TCPclient {
  public:
    TCPclient(int port);
    ~TCPclient();

    void run(int n);

  private:
    char *buffer[1024];
    sockaddr_in serverAddr;
    int epfd;
    vector<int> fds;
    void loop();
    int connectn(int n);
    int randsend(int fromfd);
    bool check(int fd, Message &msg);
};
#endif
