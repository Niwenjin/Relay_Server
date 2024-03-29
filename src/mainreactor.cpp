#include "mainreactor.h"
#include <arpa/inet.h>
#include <iostream>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAX_EVENTS 10000

MainReactor::MainReactor(int n)
    : thread_num(n), epfd(-1), listenfd(-1), fdflag(0) {
    listenfd = listensock_init();
    epfd = epoll_init();

    epoll_add_listen();

    // 创建n个子线程
    thread_init();
}

MainReactor::~MainReactor() {
    if (epfd != -1)
        close(epfd);
    if (listenfd != -1)
        close(listenfd);
    for (int i = 0; i < thread_num; i++)
        threads[i].join();
}

int MainReactor::epoll_init() {
    int epfd = epoll_create1(0);
    if (epfd == -1) {
        perror("epoll_create");
        exit(1);
    }
    return epfd;
}

void MainReactor::thread_init() {
    for (int i = 0; i < thread_num; i++) {
        subreactors.push_back(std::make_shared<SubReactor>());
        threads.push_back(thread(thread_func, subreactors[i]));
    }
}

void MainReactor::thread_func(shared_ptr<SubReactor> subreactor) {
    subreactor->run();
}

int MainReactor::listensock_init() {
    // 服务器监听的端口号
    const int serverPort = 6666;

    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd == -1) {
        perror("socket");
        exit(1);
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(serverPort);
    addr.sin_addr.s_addr = INADDR_ANY;

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
        // std::cout << "main_epfd: " << epfd << std::endl;
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

                if (add_fdpair(clientfd)) {
                    dispatch();
                    fdflag = 0;
                }
            }
        }
    }
}

int MainReactor::add_fdpair(int fd) {
    if (fdflag == 0) {
        fdpair[0] = fd;
        fdflag = 1;
        return 0;
    } else {
        fdpair[1] = fd;
        fdflag = 0;
        return 1;
    }
}

void MainReactor::dispatch() {
    int i = fdpair[0] % thread_num;
    subreactors[i]->add_relay_pair(fdpair[0], fdpair[1]);
}

void MainReactor::run() { accept_loop(); }