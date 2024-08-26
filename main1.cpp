

#include <iostream>
#include <unistd.h>
#include <cstring>
#include <termios.h>
#include <fcntl.h>
#include <errno.h>
#include <cstdlib> 
#include <ctime>   
#include <thread>
#include <pthread.h>
#include "mess.pb.h"
#include "Buffer.h"
#include <time.h>

using namespace std;

pthread_t id1, id2;
int fd0; 

Coordinate coorarr[3];
Heartbeat heararr[3];
    
int openSerialPort(const char* portname) {
    int fd = open(portname, O_RDWR | O_NOCTTY | O_SYNC);
    if (fd < 0) {
        cerr << "Error opening " << portname << ": " << strerror(errno) << endl;
        return -1;
    }
    return fd;
}

bool configureSerialPort(int fd, int speed) {
    struct termios tty;
    if (tcgetattr(fd, &tty) != 0) {
        cerr << "Error from tcgetattr: " << strerror(errno) << endl;
        return false;
    }

    cfsetospeed(&tty, speed);
    cfsetispeed(&tty, speed);

    tty.c_cflag &= ~PARENB;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;
    tty.c_cflag &= ~CRTSCTS;
    tty.c_cflag |= CREAD | CLOCAL;

    tty.c_lflag &= ~ICANON;
    tty.c_lflag &= ~ECHO;
    tty.c_lflag &= ~ECHOE;
    tty.c_lflag &= ~ECHONL;
    tty.c_lflag &= ~ISIG;
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);
    tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL);

    tty.c_oflag &= ~OPOST;
    tty.c_oflag &= ~ONLCR;

    tty.c_cc[VTIME] = 10;
    tty.c_cc[VMIN] = 1;

    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        cerr << "Error from tcsetattr: " << strerror(errno) << endl;
        return false;
    }
    return true;
}

int generateRandomNumber(int min, int max) {
    static bool initialized = false;
    if (!initialized) {
        std::srand(std::time(0));
        initialized = true;
    }
    return std::rand() % (max - min + 1) + min;
}


void *sendMessage(void *para) {
    int *i = (int *)para;
    char header;
    size_t headerSize = 1;
    int fd = *i;
    int sendcount = 0;
    time_t lastPrintTime = time(nullptr); 

    while (true) {

        int MessType = generateRandomNumber(1, 2);
        if (MessType == 1) {
            header = 'A';
            Coordinate coor;
            coor.set_x(generateRandomNumber(0,100));
            coor.set_y(generateRandomNumber(0,100));
            coor.set_z(generateRandomNumber(0,100));

            size_t size = coor.ByteSizeLong();
            char* data = new char[size];
            coor.SerializeToArray(data, size);
            write(fd, &header, headerSize);
            ssize_t written = write(fd, data, size);
            written += write(fd, "END", 3);
            if (written <= 0) {
                std::cerr << "ERROR: Failed to write to serial port" << std::endl;
            }
            // else {
            //     cout <<"Data sent:" << coor.x() <<" " << coor.y() <<" " << coor.z() << endl;
            // } 

            sendcount ++;
            delete[] data;
        } else if (MessType == 2) {
            header = 'B';
            Heartbeat heartbeat;
            heartbeat.set_panangle(static_cast<float>(generateRandomNumber(0, 50)));
            heartbeat.set_tiltangle(static_cast<float>(generateRandomNumber(0, 50)));
            for (int i = 0; i < 6; ++i) {
                heartbeat.add_errorcode(generateRandomNumber(0, 255));
            }

            size_t size = heartbeat.ByteSizeLong();
            char* data = new char[size];
            heartbeat.SerializeToArray(data, size);
            write(fd, &header, headerSize);
            ssize_t written = write(fd, data, size);
            written += write(fd, "END", 3);
            if (written  <= 0) {
                std::cerr << "ERROR: Failed to write to serial port" << std::endl;
            }

            sendcount ++;
            delete[] data;
        }

        time_t currentTime = time(nullptr);
        if (difftime(currentTime, lastPrintTime) >= 1.0) {
            cout <<"                 sendcount : " << sendcount << endl;
            sendcount = 0;
            lastPrintTime = currentTime;
        }

        usleep(1200);
    }
}

void *receiveMessage(void *para) {
    int *i = (int *)para;
    const size_t buffer_size = 32;
    char buffer[buffer_size];
    int fd = *i;
    Buffer buff(64);
    char header;
    char data[32] = {0};
    size_t dataSize;
    int receivecount = 0;
    time_t lastPrintTime1 = time(nullptr);

    while (true) {
        ssize_t bytesRead = read(fd, buffer, buffer_size);
        if (bytesRead > 0) {
            buff.pushData(buffer, bytesRead);

            while (buff.processData(header, data, sizeof(data) - 1, dataSize)) {
                if (header == 'B') {
                    Heartbeat heartbeat;
                    if (heartbeat.ParseFromArray(data, dataSize) < 0) {
                    //     cout << "Received Heartbeat message: "
                    //          << "panAngle=" << heartbeat.panangle() << ", "
                    //          << "tiltAngle=" << heartbeat.tiltangle() << ", "
                    //          << "errorCodes=[";
                    //     for (int i = 0; i < heartbeat.errorcode_size(); ++i) {
                    //         cout << (i > 0 ? ", " : "") << heartbeat.errorcode(i);
                    //     }
                    //     cout << "]" << endl;
                    // } else {
                        std::cerr << "Failed to parse Protobuf Heartbeat message" << std::endl;
                    }
                    receivecount ++;
                } else if (header == 'A') {
                    Coordinate coor;
                    if (coor.ParseFromArray(data, dataSize) < 0) {
                    //     cout << "Received Coordinate message: "
                    //          << "x=" << coor.x() << ", "
                    //          << "y=" << coor.y() << ", "
                    //          << "z=" << coor.z() << endl;
                    // } else {
                        std::cerr << "Failed to parse Protobuf Coordinate message" << std::endl;
                    }
                    receivecount ++;
                } else {
                    std::cerr << "Unexpected header value: " << static_cast<int>(header) << std::endl;
                }

                // Làm sạch dữ liệu đã được xử lý khỏi buffer
                buff.clearProcessedData();
            }
        } else if (bytesRead == 0) {
            std::cerr << "No data received" << std::endl;
        } else {
            std::cerr << "ERROR: Failed to read from serial port: " << strerror(errno) << std::endl;
        }

        time_t currentTime = time(nullptr);
        if (difftime(currentTime, lastPrintTime1) >= 1.0) {
            cout <<"                 receivecount : " << receivecount << endl;
            receivecount = 0;
            lastPrintTime1 = currentTime;
        }

        usleep(1200);
    }
}



int main() {
    GOOGLE_PROTOBUF_VERIFY_VERSION;

            int i = 300;
        coorarr[0].set_x(i + 1);
        coorarr[0].set_y(i + 2);
        coorarr[0].set_z(i + 3);
        heararr[0].set_panangle(static_cast<float>(i+1));
        heararr[0].set_tiltangle(static_cast<float>(i+2));
        for (int j = 0; j < 6; ++j) {
                heararr[0].add_errorcode(i+3);
            }
        coorarr[1].set_x(i+4);
        coorarr[1].set_y(i+5);
        coorarr[1].set_z(i+6);
        heararr[1].set_panangle(static_cast<float>(i+4));
        heararr[1].set_tiltangle(static_cast<float>(i+5));
        for (int j = 0; j < 6; ++j) {
                heararr[1].add_errorcode(i+6);
        }
        coorarr[2].set_x(i+7);
        coorarr[2].set_y(i+8);
        coorarr[2].set_z(i+9);
        heararr[2].set_panangle(static_cast<float>(i+7));
        heararr[2].set_tiltangle(static_cast<float>(i+8));
        for (int j = 0; j < 6; ++j) {
                heararr[2].add_errorcode(i+9);
        }


    const char* portname0 = "/dev/ttyUSB0";
    fd0 = openSerialPort(portname0);
    if (fd0 < 0)
        return 1;

    if (!configureSerialPort(fd0, B230400)) {
        close(fd0);
        return 1;
    }

    if (pthread_create(&id1, NULL, &sendMessage, &fd0)) {
        cout << "ERROR: Create thread 1 fail" << endl;
        return 1;
    }

    if (pthread_create(&id2, NULL, &receiveMessage, &fd0)) {
        cout << "ERROR: Create thread 2 fail" << endl;
        return 1;
    }


    pthread_join(id1, NULL);
    pthread_join(id2, NULL);

    google::protobuf::ShutdownProtobufLibrary();
    close(fd0);

    return 0;
}