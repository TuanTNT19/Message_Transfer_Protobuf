#pragma once


#include <iostream>
#include <cstring>
#include <unistd.h>
#include <errno.h>
#include <cstdlib> // Để sử dụng rand() và srand()
#include <ctime>   // Để sử dụng time()
#include <pthread.h>
#include "mess.pb.h"
#include "Buffer.h"

class ReceiveMessage {
private:

    char header;
    char data[32];
    size_t dataSize;
	Coordinate coor;

public:


    int Receive(int fd, char *buffer, size_t &byteread);
    int Parse();
};