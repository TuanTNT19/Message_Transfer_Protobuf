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
