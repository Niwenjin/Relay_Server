#include "TCPserver.h"

TcpServer::TcpServer(int n) {
    mainreactor = new MainReactor(n);
}

TcpServer::TcpServer() {
    mainreactor = new MainReactor(4);
}

TcpServer::~TcpServer() {
    delete mainreactor;
}

void TcpServer::run() {
    mainreactor->run();
}