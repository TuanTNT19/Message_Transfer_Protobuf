

#include "SendMessage.h"

#include <string>
#include <cstring>



int SendMessage ::SendCoor(int fd, Coordinate co){
    char header = 'A';
    size_t headerSize = 1;
    ssize_t written =0;
    size_t size = co.ByteSizeLong();
    char* data = new char[size];
    co.SerializeToArray(data, size); // Serialize data after the header
    written += write(fd, &header, headerSize);
    written += write(fd, data, size);
    written += write(fd, "END", 3);
    if (written > 0) {
     //   std::cout << "Data Coor sent OK :" << written << endl;
        return 1;
    } else {
        std::cerr << "ERROR: Failed to write to serial port" << std::endl;
        return 0;
    }

    delete[] data;

}

int SendMessage ::SendHeart(int fd, Heartbeat he){
    char header = 'B';
    size_t headerSize = 1;
    ssize_t written = 0; 
    size_t size = he.ByteSizeLong();
    char* data = new char[size];
    he.SerializeToArray(data, size); // Serialize data after the header
    written += write(fd, &header, headerSize);
    written += write(fd, data, size);
    written += write(fd, "END", 3);

    if (written > 0) {
    //    std::cout << "Data Heartbeat sent OK :" << written << endl;
        return 2;
    } else {
        std::cerr << "ERROR: Failed to write to serial port" << std::endl;
        return 0;
    }


    delete[] data;
}