#include "TCPserver.h"
#include <iostream>

#define SERV_PORT 6666

using std::cerr;
using std::endl;
using std::stoi;

int main(int argc, char *argv[]) {
    // if (argc != 2) {
    //     cerr << "please input thread number." << endl;
    //     return -1;
    // }
    // int n = stoi(argv[1]);
    int n = 4;
    TCPserver *server = new TCPserver(SERV_PORT, n);
    server->run();
    server->shutdown();
    delete (server);
    return 0;
}