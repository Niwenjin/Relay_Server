#include "TCPclient.h"
#include "message.h"
#include <arpa/inet.h>
#include <fcntl.h>
#include <iostream>
#include <memory.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAX_EVENTS 10000

TCPclient::TCPclient(int port) : epfd(-1) {
    epfd = epoll_create1(0);
    if (epfd == -1) {
        perror("epoll_create1");
        exit(1);
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
    this->serverAddr = serverAddr;
}

TCPclient::~TCPclient() {
    for (int fd : fds) {
        if (fd != -1)
            close(fd);
    }
    if (epfd != -1)
        close(epfd);
}

void TCPclient::run(int n) {
    connectn(n);
    loop();
}

void TCPclient::loop() {
    epoll_event events[MAX_EVENTS];
    while (true) {
        int n = epoll_wait(epfd, events, MAX_EVENTS, -1);
        if (n == -1) {
            perror("epoll_wait");
            exit(1);
        }
        for (int i = 0; i < n; ++i) {
            if (events[i].events & EPOLLOUT) {
                randsend(events[i].data.fd);
            } else if (events[i].events & EPOLLIN) {
                Message msg;
                read(events[i].data.fd, buffer, sizeof(buffer));
                memcpy(&msg, buffer, sizeof(msg));

                if (!check(events[i].data.fd, msg))
                    std::cerr << "unexpected receive!" << std::endl;
                else
                    std::cout << "receive: " << msg.data << std::endl;

            } else if (events[i].events & (EPOLLERR | EPOLLHUP | EPOLLRDHUP)) {
                close(events[i].data.fd);
                events[i].data.fd = -1;
            }
        }
    }
}

int TCPclient::connectn(int n) {
    for (int i = 0; i < n; ++i) {
        int clientFd = socket(AF_INET, SOCK_STREAM, 0);
        if (clientFd == -1) {
            perror("socket");
            exit(1);
        }
        
        if (connect(clientFd, reinterpret_cast<sockaddr *>(&serverAddr),
                    sizeof(serverAddr)) == -1) {
            perror("connect");
            exit(1);
        }

        fcntl(clientFd, F_SETFL, fcntl(clientFd, F_GETFL, 0) | O_NONBLOCK);
        epoll_event event;
        event.data.fd = clientFd;
        event.events = EPOLLOUT | EPOLLERR | EPOLLHUP | EPOLLRDHUP;
        if (epoll_ctl(epfd, EPOLL_CTL_ADD, clientFd, &event) == -1) {
            perror("epoll_ctl");
            exit(1);
        }
        fds.push_back(clientFd);
    }
    return 0;
}

int TCPclient::randsend(int fromfd) {
    int tofd = fds[rand() % fds.size()];
    while (tofd == fromfd)
        tofd = fds[rand() % fds.size()];

    Message msg;
    msg.fromfd = fromfd;
    msg.tofd = tofd;
    sprintf(msg.data, "hello, %d, I am %d", tofd, fromfd);

    char buffer[128];
    memcpy(buffer, &msg, sizeof(msg));
    if (send(fromfd, buffer, sizeof(buffer), 0) == -1) {
        perror("send");
        return -1;
    }
    return 0;
}

bool TCPclient::check(int fd, Message &msg) { return fd == msg.tofd; }