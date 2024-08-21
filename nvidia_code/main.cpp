
#include "DataProcess.h"
#include "Message.h"
#include "ReceiveMessage.h"
#include "SendMessage.h"
#include "File.h"

pthread_t id1,id2,id3;
int fd, fdc, fdh;
Message *Mess;
int temp ;
pthread_mutex_t mymutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond1 = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond2 = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond3 = PTHREAD_COND_INITIALIZER;
int phase = 1;  // 1: Thread 1, 2: Thread 2, 3: Thread 3

void *Thread_func1(void *para){
    cout << "Thread 1" << endl;
    Mess = new ReceiveMessage;
    int *i = (int *)para;
    int f = *i;
    while (1) {
        if (Mess->Receive(f)) {
            pthread_mutex_lock(&mymutex);

            temp = Mess->Parse();
            phase = 2;  // Chuyển sang trạng thái 2 sau khi Thread 1 hoàn thành
            pthread_cond_signal(&cond2);  // Đánh thức Thread 2
            pthread_mutex_unlock(&mymutex);
        }
        usleep(1250);
    }

    delete Mess;
    return NULL;
}

void *Thread_func2(void *para){
    DataProcess dp;
    while (1) {
        pthread_mutex_lock(&mymutex);

        while (phase != 2) {  // Đợi cho đến khi phase trở thành 2
            pthread_cond_wait(&cond2, &mymutex);
        }

        if (temp == 1) {
            Mess->coor = dp.CoorProcess(Mess->coor, fdc);
        } else if (temp == 2) {
            Mess->heartbeat = dp.HeartProcess(Mess->heartbeat, fdh);
        }

        phase = 3;  // Chuyển sang trạng thái 3 sau khi Thread 2 hoàn thành
        pthread_cond_signal(&cond3);  // Đánh thức Thread 3
        pthread_mutex_unlock(&mymutex);

        usleep(1250);
    }

    return NULL;
}

void *Thread_func3(void *para){
    SendMessage SendMsg;
    int *i = (int *)para;
    int f = *i;
    while (1) {
        pthread_mutex_lock(&mymutex);

        while (phase != 3) {  // Đợi cho đến khi phase trở thành 3
            pthread_cond_wait(&cond3, &mymutex);
        }

        if (temp == 1) {
            SendMsg.SendCoor(f, Mess->coor);
        } else if (temp == 2) {
            SendMsg.SendHeart(f, Mess->heartbeat);
        }

        phase = 1;  // Chuyển lại trạng thái 1 sau khi Thread 3 hoàn thành
        pthread_cond_signal(&cond1);  // Đánh thức Thread 1
        pthread_mutex_unlock(&mymutex);

        usleep(1250);
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

    while(1);


    close(fdc);
    close(fdh);
    close(fd);

    return 0;
}

