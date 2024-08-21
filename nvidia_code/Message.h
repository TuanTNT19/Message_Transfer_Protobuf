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
	virtual bool Receive(int fd) ;
	virtual int Parse ();
};