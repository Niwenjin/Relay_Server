#include "socket.h"
#include <cstring>
#include <iostream>
#include <sys/socket.h>

#define SERV_PORT 9877
#define LISTENQ 1024
#define SERV_IP "127.0.0.1"

using std::cerr;
using std::cin;
using std::cout;
using std::endl;

void Server::init() {
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        cerr << "create socket error";

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);
    if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0)
        cerr << "bind error.";
    else
        cout << "listen on port: " << SERV_PORT << endl;

    listen(sockfd, LISTENQ);
}

void Server::Accept() {
    cout << "waiting for connect..." << endl;
    clientlen = sizeof(clientlen);
    connfd = accept(sockfd, (struct sockaddr *)&clientaddr, &clientlen);
}

void Server::Send() { write(connfd, buf, n); }

void Server::Recv() {
    n = read(connfd, buf, sizeof(buf));
    cout << "receive:" << buf << endl;
}

void Server::Close() {
    if (sockfd != -1) {
        close(sockfd);
        sockfd = -1;
    }
    if (connfd != -1) {
        close(connfd);
        connfd = -1;
    }
}

void Client::init() {
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        cerr << "create socket error";
}

void Client::Connect() {
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    inet_pton(AF_INET, SERV_IP, &servaddr.sin_addr);
    servaddr.sin_port = htons(SERV_PORT);

    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0)
        cerr << "connect error.";
}

void Client::Send() { write(sockfd, buf, strlen(buf)); }

void Client::Recv() { n = read(sockfd, buf, sizeof(buf)); }

void Client::input() { cin.getline(buf, sizeof(buf)); }

void Client::show() { cout << buf << endl; }

void Client::Close() {
    if (sockfd != -1) {
        close(sockfd);
        sockfd = -1;
    }
}