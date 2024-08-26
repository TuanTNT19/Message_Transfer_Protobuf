#pragma once

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <errno.h>
#include <cstdlib> // Để sử dụng rand() và srand()
#include <ctime>   // Để sử dụng time()
#include <pthread.h>
#include "mess.pb.h"

using namespace std;

class Message{

	public:

	Coordinate coor;
	Heartbeat heartbeat;
	virtual int Receive(int fd, char *buffer, size_t &byteread);
	//virtual int Parse ();
};