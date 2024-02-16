#ifndef MESSAGE_H
#define MESSAGE_H

class Message {
  public:
    Message() : fromfd(-1), tofd(-1){};
    ~Message(){};
    int fromfd;
    int tofd;
    char data[64];
};
#endif