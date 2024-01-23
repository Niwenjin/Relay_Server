#include "task.h"
#include <arpa/inet.h>
#include <fcntl.h>
#include <iostream>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFFER_SIZE 128
#define ACCEPT 0
#define ECHO 1

using std::cout;
using std::endl;

Task::Task(int epfd, int type, int fd, char *buf)
    : epfd(epfd), fd(fd), buf(buf) {
    if (type == ACCEPT) {
        // Accept事件
        func = [this](int epfd, int fd, char *buf) {
            struct sockaddr_in clientAdr;
            socklen_t len = sizeof(clientAdr);
            int connfd = accept(fd, (struct sockaddr *)&clientAdr, &len);
            if (connfd < 0) {
                perror("accept");
                return;
            }

            // 设置连接的套接字为非阻塞
            if (fcntl(connfd, F_SETFL, O_NONBLOCK) < 0) {
                perror("fcntl");
                close(connfd);
            }

            epoll_event newevent;
            newevent.events = EPOLLIN | EPOLLOUT | EPOLLET;
            newevent.data.fd = connfd;
            epoll_ctl(epfd, EPOLL_CTL_ADD, connfd, &newevent);

            cout << "connfd: " << connfd << endl;
        };
    } else if (type == ECHO) {
        // Echo事件
        func = [this](int epfd, int fd, char *buf) {
            ssize_t n = recv(fd, buf, BUFFER_SIZE, 0);

            if (n > 0) {
                buf[n] = '\0';
                cout << "recvform fd: " << fd << endl;

            } else if (n == 0) {
                // 连接断开
                epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
                close(fd);

            } else {
                // 处理接收错误
                perror("receive");
            }
            send(fd, buf, n, 0);
        };
    }
}

void Task::run() { func(epfd, fd, buf); }