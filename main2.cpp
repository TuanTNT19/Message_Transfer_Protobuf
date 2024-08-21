#include <iostream>
#include <unistd.h>
#include <cstring>
#include <termios.h>
#include <fcntl.h>
#include <errno.h>
#include <cstdlib> // Để sử dụng rand() và srand()
#include <ctime>   // Để sử dụng time()
#include <thread>
#include "mess.pb.h"

using namespace std;

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
    // Khởi tạo bộ sinh số ngẫu nhiên chỉ một lần
    static bool initialized = false;
    if (!initialized) {
        std::srand(std::time(0)); // Khởi tạo với thời gian hiện tại
        initialized = true;
    }

    // Phát sinh số ngẫu nhiên trong khoảng từ min đến max
    return std::rand() % (max - min + 1) + min;
}

void sendMessage(int fd ) {
    char header;
    size_t headerSize = 1;

    while (true) {
        int MessType = generateRandomNumber(1,2);
        if ( MessType == 1){
        header = 'A';
        Coordinate coor;
        coor.set_x(generateRandomNumber(0,100));
        coor.set_y(generateRandomNumber(0,100));
        coor.set_z(generateRandomNumber(0,100));

        size_t size = coor.ByteSizeLong();
        char* data = new char[size];
        coor.SerializeToArray(data, size); // Serialize data after the header
        write(fd, &header, headerSize);
        ssize_t written = write(fd, data, size);
        if (written > 0) {
            std::cout << "Data Coor sent successfully, bytes written: " << written <<". Data: ";
            cout <<coor.x()<<" "<<coor.y() <<" "<<coor.z()<<endl;
        } else {
            std::cerr << "ERROR: Failed to write to serial port" << std::endl;
        }

        delete[] data;
        }

        else if (MessType == 2){
            header = 'B'; // Header cho bản tin 2
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
            if (written > 0) {
                std::cout << "Data Hearbeat sent successfully, bytes written: " << written <<". Data: ";
                cout << heartbeat.panangle()<<" "<<heartbeat.tiltangle() <<endl;
            } else {
                std::cerr << "ERROR: Failed to write to serial port" << std::endl;
            }

            delete[] data;          
        }

        usleep(1250);
    }
}


void receiveMessage(int fd) {
    const size_t buffer_size = 1024;
    char buffer[buffer_size];

    while (true) {
        ssize_t bytesRead = read(fd, buffer, buffer_size);
        if (bytesRead > 0) {
            // Xử lý header
            char header = buffer[0];
            if (header == 'A') { // Kiểm tra giá trị của header
                Coordinate coor;
                size_t dataSize = bytesRead - 1; // kích thước dữ liệu Protobuf
                if (dataSize > 0 && coor.ParseFromArray(buffer + 1, dataSize)) {
                    cout << "Received Coordinate message: "
                         << "x=" << coor.x() << ", "
                         << "y=" << coor.y() << ", "
                         << "z=" << coor.z() << endl;
                    //cout << "Bytes read = " << bytesRead << endl;
                } else {
                    std::cerr << "Failed to parse Protobuf Coordinate message" << std::endl;
                }
            } 
            else if(header == 'B'){
                Heartbeat heartbeat;
                size_t dataSize = bytesRead - 1;
                if (dataSize > 0 && heartbeat.ParseFromArray(buffer + 1, dataSize)) {
                    cout << "Received Heartbeat message: "
                         << "panAngle=" << heartbeat.panangle() << ", "
                         << "tiltAngle=" << heartbeat.tiltangle() << ", "
                         << "errorCodes=[";
                    for (int i = 0; i < heartbeat.errorcode_size(); ++i) {
                        cout << (i > 0 ? ", " : "") << heartbeat.errorcode(i);
                    }
                    cout << "]" << endl;
                }
                else {
                    std::cerr << "Failed to parse Protobuf Heartbeat message" << std::endl;
                }
            }

           else {
                std::cerr << "Unexpected header value: " << static_cast<int>(header) << std::endl;
            }

        } else if (bytesRead == 0) {
            std::cerr << "No data received" << std::endl;
        } else {
            std::cerr << "ERROR: Failed to read from serial port: " << strerror(errno) << std::endl;
        }

        usleep(1250);
    }
}


int main() {
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    const char* portname1 = "/dev/ttyV1";

    int fd1 = openSerialPort(portname1);
    if (fd1 < 0)
        return 1;

    if (!configureSerialPort(fd1, B115200)) {
        close(fd1);
        return 1;
    }

    thread thr1(receiveMessage, fd1);
    thread thr2(sendMessage, fd1);

    thr1.join(); // Đợi luồng gửi hoàn tất
    thr2.join(); // Đợi luồng nhận hoàn tất

    google::protobuf::ShutdownProtobufLibrary();

    close(fd1);


    return 0;
}
