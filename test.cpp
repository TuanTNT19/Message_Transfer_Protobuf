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

class Message{
	protected:
	public:

	virtual int openSerialPort(char *portname) = 0;
	virtual bool configureSerialPort(int fd, int speed) ;

};

bool Message::configureSerialPort(int fd, int speed){

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
    tty.c_cc[VMIN] = 1; // Đảm bảo đọc ít nhất một byte

    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        cerr << "Error from tcsetattr: " << strerror(errno) << endl;
        return false;
    }
    return true;
}

class ReceiveMessage : public Message{
	private:
	char buffer[1024];
	const size_t buffer_size = 1024;
	ssize_t bytesRead ;

	public:
	void Receive(int fd);
	void Parse ();

};

void ReceiveMessage::Receive(int fd){
	bytesRead = read(fd, buffer, buffer_size);
	if (bytesRead == 0){
		cout <<"No data receive"<<endl;
	}
	else if(bytesRead < 0){
		cout <<"ERROR: Failed to read "<<endl;
	}
}

void ReceiveMessage :: Parse(){
            char header = buffer[0];
            if (header == 'A') { // Kiểm tra giá trị của header
                Coordinate coor;
                size_t dataSize = bytesRead - 1; // kích thước dữ liệu Protobuf
                if (dataSize > 0 && coor.ParseFromArray(buffer + 1, dataSize)) {
                    cout << "Received message: "
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
}

int main(){
	return 0;
}