#include "ReceiveMessage.h"

int countCharsBetweenAB(const char* arr) {
    int t = -1;
    for (int i = 0; i < 32; i++){
        if ((arr[i] == 'E') && (arr[i + 1] == 'N') && (arr[i + 2] == 'D')){
            t = i;
            break;
        }
    }

    return (t - 1);
}

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
                size_t dataSize = countCharsBetweenAB(buffer); // kích thước dữ liệu Protobuf
                if (dataSize > 0 && coor.ParseFromArray(buffer + 1, dataSize)) {
                        return 1;
                } else {
                    std::cerr << "Failed to parse Protobuf Coordinate message" << std::endl;
                    return 0;
                }
            } 
            else if(header == 'B'){
                size_t dataSize = countCharsBetweenAB(buffer);
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