#include "ReceiveMessage.h"

bool ReceiveMessage::Receive(int fd){
	bytesRead = read(fd, buffer, buffer_size);
    cout <<"ByteRead: " << bytesRead << endl;
	if (bytesRead == 0){
		cout <<"No data receive"<<endl;
        return 0;
	}
	else if(bytesRead < 0){
		cout <<"ERROR: Failed to read "<<endl;
        return 0;
	}

    return 1;
}

int ReceiveMessage :: Parse(){
            char header = buffer[0];
            if (header == 'A') { // Kiểm tra giá trị của header
                size_t dataSize = bytesRead - 1; // kích thước dữ liệu Protobuf
                if (dataSize > 0 && coor.ParseFromArray(buffer + 1, dataSize)) {
                        return 1;
                } else {
                    std::cerr << "Failed to parse Protobuf Coordinate message" << std::endl;
                    return 0;
                }
            } 
            else if(header == 'B'){
                size_t dataSize = bytesRead - 1;
                if (dataSize > 0 && heartbeat.ParseFromArray(buffer + 1, dataSize)) {
                    
                    return 2;
                }
                else {
                    std::cerr << "Failed to parse Protobuf Heartbeat message" << std::endl;
                    return 0;
                }
            }

           else {
                std::cerr << "Unexpected header value: " << static_cast<int>(header) << std::endl;
                return -1;
            }
}