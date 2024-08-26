#pragma once

#include "Message.h"
#include "Buffer.h"

class ReceiveMessage : public Message {
private:
    //char buffer[1024];
    //const size_t buffer_size = 1024;
    //ssize_t bytesRead;
 //   Buffer buff; // Khởi tạo Buffer trong constructor
    char header;
    char data[32];
    size_t dataSize;

public:
    // Constructor mặc định
    // ReceiveMessage() : buff(64), dataSize(0) {
    //     // Có thể khởi tạo thêm các giá trị nếu cần
    // }

    // // Constructor với kích thước tùy chỉnh
    // ReceiveMessage(size_t bufferCapacity) : buff(bufferCapacity), dataSize(0) {
    //     // Có thể khởi tạo thêm các giá trị nếu cần
    // }

    int Receive(int fd, char *buffer, size_t &byteread);
    int Parse();
};