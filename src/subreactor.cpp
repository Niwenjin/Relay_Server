#include "subreactor.h"
#include "task.h"
#include "sock_item.h"
#include <iostream>
#include <sys/epoll.h>
#include <unistd.h>

SubReactor::SubReactor() : epfd(-1) { epfd = epoll_init(); }

SubReactor::~SubReactor() {
    if (epfd != -1)
        close(epfd);
}

int SubReactor::epoll_init() {
    int epfd = epoll_create1(0);
    if (epfd == -1) {
        perror("epoll_create");
        exit(1);
    }
    return epfd;
}

void SubReactor::add_event(int fd) {
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = fd;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev) == -1) {
        perror("epoll_ctl");
        exit(1);
    }
    
    sock_map[fd] = Sock_item(fd);
}

void SubReactor::run() { relay_loop(); }

void SubReactor::relay_loop() {
    while (1) {
        struct epoll_event events[10];
        int nfds = epoll_wait(epfd, events, 10, -1);
        if (nfds == -1) {
            perror("epoll_wait");
            exit(1);
        }

        for (int i = 0; i < nfds; i++) {
            int fd = events[i].data.fd;
            // 转发
            relay_event(fd);
        }
    }
}

void SubReactor::relay_event(int fd) {
    // 转发
    Task task(epfd, 2, fd, sock_map[fd]);
    task.run();
}