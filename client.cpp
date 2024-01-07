#include "socket.h"

int main() {
    Client *cli = new Client();
    cli->Connect();
    while (true) {
        cli->input();
        cli->Send();
        cli->Recv();
        cli->show();
    }
    cli->Close();
    delete (cli);
}