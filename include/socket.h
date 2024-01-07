#ifndef SOCKET_H
#define SOCKET_H

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFSIZE 1024

class Socket {
  public:
    Socket() : sockfd(-1){};
    virtual ~Socket() {
        if (sockfd != -1)
            close(sockfd);
    }
    virtual void init(){};
    virtual void Send(){};
    virtual void Recv(){};
    virtual void Close(){};
    int sockfd;
};

class Server : public Socket {
  public:
    Server() { init(); }
    void Accept();
    void Send() override;
    void Recv() override;
    void Close() override;

  private:
    void init() override;
    struct sockaddr_in servaddr, clientaddr;
    socklen_t clientlen;
    int connfd;
    char buf[BUFSIZE];
    int n;
};

class Client : public Socket {
  public:
    Client() { init(); }
    void Connect();
    void Send() override;
    void Recv() override;
    void Close() override;
    void input();
    void show();

  private:
    void init() override;
    struct sockaddr_in servaddr;
    char buf[BUFSIZE];
    int n;
};

#endif