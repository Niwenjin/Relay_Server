#include "TCPserver.h"
#include <iostream>

#define SERV_PORT 6666

int main() {
    TCPserver *server = new TCPserver(SERV_PORT);
    server->run();
    server->shutdown();
    delete (server);
    return 0;
}