#include "TCPclient.h"
#include <iostream>

const int SERVER_PORT = 6666;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "Input number of clients." << std::endl;
        return -1;
    }
    TCPclient client(SERVER_PORT);
    client.run(atoi(argv[1]));
    return 0;
}