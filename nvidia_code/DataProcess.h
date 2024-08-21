#pragma once

#include <iostream>
#include <unistd.h>
#include <cstring>
//#include <sstream>
#include "mess.pb.h"

using namespace std;

class DataProcess{
    private:
    //static const size_t BUFFER_SIZE = 256;

    public:
    Coordinate CoorProcess(const Coordinate& c, int fd);
    Heartbeat HeartProcess(const Heartbeat& h, int fd);
};