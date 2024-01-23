#include "reactor.h"
#include "threadpool.h"
#include <arpa/inet.h>
#include <cstddef>
#include <cstdio>
#include <fcntl.h>
#include <iostream>
#include <memory.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAX_EPOLL_SIZE 65536
#define ACCEPT 0
#define ECHO 1

using std::cerr;
// using std::cout;
using std::endl;

Reactor::Reactor(int fd, int n) : listenfd(fd) {
    epfd = init_reactor();
    pool = new ThreadPool(n);
}

Reactor::~Reactor() {
    if (epfd != -1) {
        close(epfd);
        epfd = -1;
    }
    delete (pool);
}

int Reactor::init_reactor() {
    epfd = epoll_create(1);
    if (epfd <= 0) {
        cerr << "epoll_create() failed, errno: " << errno << endl;
        return -1;
    }
    return epfd;
}

int Reactor::reactor_loop() {
    epoll_event events[MAX_EPOLL_SIZE];
    memset(events, 0, sizeof(events));
    // 添加listenfd到epoll的监听队列
    // cout << "listenfd: " << listenfd << endl;
    set_event(listenfd, EPOLLIN | EPOLLET);

    int nready = 0;
    while (1) {
        // cout << "epfd: " << epfd << endl;
        nready = epoll_wait(epfd, events, MAX_EPOLL_SIZE, -1);
        if (nready == -1) {
            perror("epoll_wait error");
            break;
        }

        for (int i = 0; i < nready; ++i) {
            if (events[i].events & EPOLLIN) {
                int rfd = events[i].data.fd;
                if (rfd <= 0) {
                    cerr << "invaild rfd." << endl;
                    continue;
                }
                // cout << "rfd: " << rfd << endl;
                if (rfd == listenfd) {
                    // Accept事件
                    // cout<< "accept event"<<endl;
                    pool->add_task(epfd, ACCEPT, rfd);
                } else {
                    // Read事件
                    // cout<< "read event"<<endl;
                    pool->add_task(epfd, ECHO, rfd);
                }
            }
            if (events[i].events & EPOLLOUT) {
                // Write事件
                int wfd = events[i].data.fd;
                if (event_write(wfd) == -1)
                    cerr << "write fd: " << wfd << " error." << endl;
            }
        }
    }

    return 0;
}

int Reactor::set_event(int fd, int event) {
    epoll_event newevent;
    newevent.events = event;
    newevent.data.fd = fd;
    return epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &newevent);
}

int Reactor::del_event(int fd) {
    return epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
}

int Reactor::event_accept(int fd) {
    struct sockaddr_in clientAdr;
    socklen_t len = sizeof(clientAdr);
    int connfd = accept(fd, (struct sockaddr *)&clientAdr, &len);
    if (connfd < 0) {
        perror("accept");
        return -1;
    }

    // 设置连接的套接字为非阻塞
    if (fcntl(connfd, F_SETFL, O_NONBLOCK) < 0) {
        perror("fcntl");
        close(connfd);
        return -1;
    }

    return connfd;
}

int Reactor::event_read(int fd) {
    pool->add_task(epfd, ECHO, fd);
    return 0;
}

int Reactor::event_write(int fd) { return 0; }