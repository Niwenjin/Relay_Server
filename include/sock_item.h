#ifndef SOCK_ITEM_H
#define SOCK_ITEM_H

#define BUFFER_SIZE 1024

class Sock_item {
  public:
    Sock_item();
    ~Sock_item();

    char *w_buf;
    int w_len;
    char *r_buf;
    int r_len;
};

#endif