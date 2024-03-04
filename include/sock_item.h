#ifndef SOCK_ITEM_H
#define SOCK_ITEM_H

#define BUFFER_SIZE 1024

class Sock_item {
  public:
    Sock_item() : fd(-1) {}
    Sock_item(int fd) : fd(fd) {}
    ~Sock_item(){};
    int fd;
    char buf[BUFFER_SIZE];
};
#endif