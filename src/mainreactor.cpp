#include "mainreactor.h"
#include <arpa/inet.h>
#include <iostream>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAX_EVENTS 10000

MainReactor::MainReactor(int n) : thread_num(n), epfd(-1), listenfd(-1) {
    epfd = epoll_init();

    listenfd = listensock_init();

    epoll_add_listen();

    // 创建n个子线程
    if (thread_init() == -1) {
        perror("thread_init");
        exit(1);
    }
}

MainReactor::~MainReactor() {
    if (epfd != -1)
        close(epfd);
    for (int i = 0; i < thread_num; i++)
        threads[i].join();
}

int MainReactor::thread_init() {
    for (int i = 0; i < thread_num; i++) {
        SubReactor subreactor;
        subreactors.push_back(subreactor);
        threads.push_back(thread(&SubReactor::run, &subreactors[i]));
    }
    return 0;
}

int MainReactor::listensock_init() {
    // 服务器监听的端口号
    const int serverPort = 6666;

    // 服务器监听的特定IP地址
    const char *serverIP = "192.168.1.2";

    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd == -1) {
        perror("socket");
        exit(1);
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(serverPort);
    addr.sin_addr.s_addr = INADDR_ANY;

    // 将IP地址从字符串形式转换为二进制形式
    if (inet_pton(AF_INET, serverIP, &addr.sin_addr) <= 0) {
        perror("Invalid IP address");
        close(listenfd);
        exit(1);
    }

    // bind
    if (bind(listenfd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        perror("bind");
        exit(1);
    }
    // listen
    if (listen(listenfd, 10) == -1) {
        perror("listen");
        exit(1);
    }

    return listenfd;
}

void MainReactor::epoll_add_listen() {
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = listenfd;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &ev) == -1) {
        perror("epoll_ctl");
        exit(1);
    }
}

void MainReactor::accept_loop() {
    while (1) {
        struct epoll_event events[10];
        int nfds = epoll_wait(epfd, events, 10, -1);
        if (nfds == -1) {
            perror("epoll_wait");
            exit(1);
        }

        for (int i = 0; i < nfds; i++) {
            if (events[i].data.fd == listenfd) {
                struct sockaddr_in client_addr;
                socklen_t client_len = sizeof(client_addr);
                int clientfd = accept(listenfd, (struct sockaddr *)&client_addr,
                                      &client_len);
                if (clientfd == -1) {
                    perror("accept");
                    exit(1);
                }

                dispatch(clientfd);
            }
        }
    }
}

void MainReactor::dispatch(int fd) {
    int i = fd % thread_num;
    subreactors[i].add_event(fd);
}

void MainReactor::run() { accept_loop(); }