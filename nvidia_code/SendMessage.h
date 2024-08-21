#pragma once

#include "Message.h"

class SendMessage {
    private:

    public:
    int SendCoor(int fd, Coordinate co);
    int SendHeart(int fd, Heartbeat he); 
};