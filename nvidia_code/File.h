#pragma once

#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <cstring>
#include <errno.h>

using namespace std;


int openTextFile(const char *filename);

int openSerialPort(const char* portname) ;

bool configureSerialPort(int fd, int speed);

