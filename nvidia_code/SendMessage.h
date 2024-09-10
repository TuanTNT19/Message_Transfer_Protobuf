#pragma once


#include <iostream>
#include <cstring>
#include <unistd.h>
#include <errno.h>
#include <cstdlib> // Để sử dụng rand() và srand()
#include <ctime>   // Để sử dụng time()
#include <pthread.h>
#include "mess.pb.h"

class SendMessage {
    private:

    public:
    int SendCoor(int fd, Coordinate co);
    int SendHeart(int fd, Heartbeat he); 
};