#include "socket.h"

int main(){
    Server *serv=new Server();
    serv->Accept();
    while (true) {
        serv->Recv();
        serv->Send();
    }
    serv->Close();
    delete (serv);
}