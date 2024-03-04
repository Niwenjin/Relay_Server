#include "subreactor.h"
#include "sock_item.h"
// #include "task.h"
#include <iostream>
#include <sys/epoll.h>
#include <unistd.h>

#define MAX_EVENTS 10000

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

void SubReactor::add_relay_pair(int fd1, int fd2) {
    epoll_add(fd1);
    epoll_add(fd2);

    sock_map[fd1] = Sock_item(fd1);
    sock_map[fd2] = Sock_item(fd2);

    relay_map[fd1] = fd2;
    relay_map[fd2] = fd1;
}

void SubReactor::run() { relay_loop(); }

void SubReactor::relay_loop() {
    while (1) {
        struct epoll_event events[MAX_EVENTS];
        // std::cout << "sub_epfd: " << epfd << std::endl;
        int nfds = epoll_wait(epfd, events, MAX_EVENTS, -1);
        if (nfds == -1) {
            perror("sub_epoll_wait");
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
    // Task task(epfd, 2, fd, sock_map[fd]);
    // task.run();
    char *buf = sock_map[fd].buf;
    int n = read(fd, buf, BUFFER_SIZE);
    if (n == -1) {
        perror("read");
        exit(1);
    }
    if (n == 0) {
        close(fd);
        close(relay_map[fd]);
        return;
    }
    write(relay_map[fd], buf, n);
}

void SubReactor::epoll_add(int fd) {
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = fd;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev) == -1) {
        perror("epoll_ctl");
        exit(1);
    }
}