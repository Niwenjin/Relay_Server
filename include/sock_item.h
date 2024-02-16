#ifndef SOCK_ITEM_H
#define SOCK_ITEM_H

#define BUFFER_SIZE 128

class Sock_item {
  public:
    Sock_item(int fd) : fd(fd) {}
    ~Sock_item(){};
    int fd;
    char buf[BUFFER_SIZE];
};
#endif