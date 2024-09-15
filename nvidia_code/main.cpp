#include "DataProcess.h"
#include "ReceiveMessage.h"
#include "SendMessage.h"
#include "File.h"
#include "Buffer.h"
#include "safeQueue.h"

pthread_t id1, id2, id3;
int fd, fdc, fdh;


Buffer buff(72);
SafeQueue<int, Coordinate, Heartbeat> mq;
SafeQueue<int, Coordinate, Heartbeat> mqt;

void *Thread_func1(void *para){
    cout << "Thread 1" << endl;
    ReceiveMessage Mess;
    int *i = (int *)para;
    int f = *i;
    char buffread[32];
    char header;
    char data[32] = {0};
    size_t dataSize;
    size_t bytesRead;

    while (1) {
        Mess.Receive(fd, buffread, bytesRead);
            buff.pushData(buffread, bytesRead);
            while (buff.processData(header, data, sizeof(data) - 1, dataSize)) {
                if (header == 'B') {
                    Heartbeat heartbeat;
                    if (heartbeat.ParseFromArray(data, dataSize)) {
                        mq.enqueue(1);
                        mq.enqueue(heartbeat);   
                    } else {
                        std::cerr << "Failed to parse Protobuf Heartbeat message" << std::endl;
                    }
                } else if (header == 'A') {
                    Coordinate coor;
                    if (coor.ParseFromArray(data, dataSize)) {
                        mq.enqueue(2);
                        mq.enqueue(coor);
                    } else {
                        std::cerr << "Failed to parse Protobuf Coordinate message" << std::endl;
                    }
                } else {
                    std::cerr << "Unexpected header value: " << static_cast<int>(header) << std::endl;
                }
                // Làm sạch dữ liệu đã được xử lý khỏi buffer (nếu cần)
                 buff.clearProcessedData();
            }            

        usleep(1200);
    }

   // delete Mess;
    return NULL;
}

void *Thread_func2(void *para){
    DataProcess dp;
    while (1) {
        auto header = mq.dequeue();    
        int check = get <int> (header);
        if (check == 2){
            auto data = mq.dequeue();
            Coordinate coor2 = get<Coordinate>(data);
            coor2 = dp.CoorProcess(coor2, fdc);
            mqt.enqueue(2);
            mqt.enqueue(coor2);
        } else if (check == 1){
            auto data = mq.dequeue();
            Heartbeat heartbeat2 = get<Heartbeat>(data); 
            heartbeat2 = dp.HeartProcess(heartbeat2, fdh);          
            mqt.enqueue(1);
            mqt.enqueue(heartbeat2);
        }
    }
    return NULL;
}

void *Thread_func3(void *para){
    SendMessage SendMsg;
    int *i = (int *)para;
    int f = *i;

    while (1) {
        auto header = mqt.dequeue();
        int check = get <int> (header);
        if (check == 1){
            auto data = mqt.dequeue();
            Heartbeat heartbeat3 = get<Heartbeat>(data);
            SendMsg.SendHeart(f, heartbeat3);
        } else if (check == 2){
            auto data = mqt.dequeue();
            Coordinate coor3 = get<Coordinate>(data);
            SendMsg.SendCoor(f, coor3);            
        }
    }
    return NULL;
}

int main(){
    fdc = openTextFile("CoordinateFile.txt");
    fdh = openTextFile("HeartbeatFile.txt");
    fd = openSerialPort("/dev/ttyTHS0");
    configureSerialPort(fd, B230400);

    if (pthread_create(&id1, NULL, &Thread_func1, &fd)){
        cout <<"ERROR : Create thread 1 fail" <<endl;
    }
    
    if (pthread_create(&id2, NULL, &Thread_func2, NULL)){
        cout <<"ERROR : Create thread 2 fail" <<endl;
    }
    
    if (pthread_create(&id3, NULL, &Thread_func3, &fd)){
        cout <<"ERROR : Create thread 3 fail" <<endl;
    }

    // Chờ các luồng kết thúc
    pthread_join(id1, NULL);
    pthread_join(id2, NULL);
    pthread_join(id3, NULL);

    close(fdc);
    close(fdh);
    close(fd);

    return 0;
}
