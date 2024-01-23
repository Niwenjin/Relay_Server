#include "TCPserver.h"
#include "reactor.h"
#include <arpa/inet.h>
#include <fcntl.h>
#include <iostream>
#include <memory.h>
#include <sys/socket.h>
#include <unistd.h>

using std::cerr;
using std::cout;
using std::endl;

TCPserver::TCPserver(int port, int n) {
    listenfd = init_socket(port);
    reactor = new Reactor(listenfd, n);
}

TCPserver::~TCPserver() {
    if (listenfd != -1) {
        close(listenfd);
        listenfd = -1;
    }
    delete (reactor);
}

int TCPserver::init_socket(int port) {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        cerr << "socket() failed, errno: " << errno;
        return -1;
    }
    // 设置套接字非阻塞
    if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0) {
        cerr << "fcntl() failed, errno: " << errno;
        return -1;
    }

    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    // 设置端口复用
    int reuseAddr = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (const void *)&reuseAddr,
               sizeof(reuseAddr));

    if (bind(fd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        cerr << "bind() failed, errno: " << errno;
        return -1;
    }

    if (listen(fd, SOMAXCONN) < 0) {
        cerr << "listen() failed, errno: " << errno;
        return -1;
    }

    return fd;
}

void TCPserver::run() {
    cout << "Reactor run." << endl;
    reactor->reactor_loop();
}

void TCPserver::shutdown() {
    if (listenfd != -1) {
        close(listenfd);
        listenfd = -1;
    }
}