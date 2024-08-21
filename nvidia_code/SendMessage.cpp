

#include "SendMessage.h"

#include <string>
#include <cstring>


void Padding( char *data, int size){
    for (int i = 0; i < size; i++){
        data[i] = '0';
    }
}

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
    int paddingSize = 32 - size - headerSize - 3;
    char* arrpadd = new char[paddingSize];
    Padding(arrpadd, paddingSize);  // Thêm padding
    written += write(fd, arrpadd, paddingSize);  // Gửi padding
    if (written > 0) {
        std::cout << "Data Coor sent OK :" << written << endl;
        return 1;
    } else {
        std::cerr << "ERROR: Failed to write to serial port" << std::endl;
        return 0;
    }

    delete[] arrpadd;
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
    int paddingSize = 32 - size - headerSize - 3;
    char* arrpadd = new char[paddingSize];
    Padding(arrpadd, paddingSize);  // Thêm padding
    written += write(fd, arrpadd, paddingSize);  // Gửi padding
    if (written > 0) {
        std::cout << "Data Heartbeat sent OK :" << written << endl;
        return 2;
    } else {
        std::cerr << "ERROR: Failed to write to serial port" << std::endl;
        return 0;
    }

    delete[] arrpadd;
    delete[] data;
}
