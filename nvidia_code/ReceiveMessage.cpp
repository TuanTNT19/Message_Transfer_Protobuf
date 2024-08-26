#include "ReceiveMessage.h"


int ReceiveMessage::Receive(int fd, char *buffer, size_t &byteread){
	byteread = read(fd, buffer, 32);
   // cout <<"ByteRead: " << bytesRead << endl;
	if (byteread == 0){
		cout <<"No data receive"<<endl;
        return 0;
	}
	else if(byteread < 0){
		cout <<"ERROR: Failed to read "<<endl;
        return 0;
	}
//buff.pushData(buffer, bytesRead);

    return 1;
}


// int ReceiveMessage :: Parse(){
//                 while (buff.processData(header, data, sizeof(data) - 1, dataSize)) {
//                 if (header == 'B') {
//                     if (heartbeat.ParseFromArray(data, dataSize)) {
//                     return 2;
//                     } else {
//                         std::cerr << "    Failed to parse Protobuf Heartbeat message" << std::endl;
//                         return 0;
//                     }
//                 } else if (header == 'A') {
//                     if (coor.ParseFromArray(data, dataSize)) {
//                         return 1;
//                     } else {
//                         std::cerr << "     Failed to parse Protobuf Coordinate message" << std::endl;
//                         return 1;
//                     }
//                 } else {
//                     std::cerr << "Unexpected header value: " << static_cast<int>(header) << std::endl;
//                 }

//                 // Làm sạch dữ liệu đã được xử lý khỏi buffer
//                 buff.clearProcessedData();
//             }
// }