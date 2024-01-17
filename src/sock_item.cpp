#include "sock_item.h"

Sock_item::Sock_item() {
    w_buf = new char[BUFFER_SIZE];
    r_buf = new char[BUFFER_SIZE];
}

Sock_item::~Sock_item() {
    delete[] w_buf;
    delete[] r_buf;
}