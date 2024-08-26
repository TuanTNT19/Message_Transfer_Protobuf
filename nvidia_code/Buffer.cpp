#include "Buffer.h"


bool Buffer::pushData(const char* data, size_t size) {
        if (size > buffer.size()) {
            std::cerr << "Data size exceeds buffer capacity!" << std::endl;
            return false;
        }

        // Nếu buffer không đủ chỗ, di chuyển dữ liệu chưa xử lý lên đầu
        if (tail + size > buffer.size()) {
            if (head > 0) {
                std::memmove(&buffer[0], &buffer[head], tail - head);
                tail -= head;
                head = 0;
            }
        }

        // Nếu sau khi di chuyển vẫn không đủ chỗ, báo lỗi
        if (tail + size > buffer.size()) {
            std::cerr << "Buffer overflow!" << std::endl;
            return false;
        }

        // Copy dữ liệu vào buffer
        std::memcpy(&buffer[tail], data, size);
        tail += size;
        return true;
    }

bool Buffer::processData(char& header, char* data, size_t dataSize, size_t &actualDataSize) {
   if (head >= tail) {
//        std::cerr << "Buffer is empty!" << std::endl;
        return false;
    }

    // Tìm header trong buffer ('A' hoặc 'B')
    size_t headerPos = head;
    while (headerPos < tail && buffer[headerPos] != 'A' && buffer[headerPos] != 'B') {
        headerPos++;
    }

    if (headerPos >= tail) {
//        std::cerr << "Header 'A' or 'B' not found!" << std::endl;
        return false;
    }

    header = buffer[headerPos];

    // Tìm footer "END" trong buffer sau header
    const char footer[] = "END";
    size_t footerStart = std::string(&buffer[headerPos], tail - headerPos).find(footer);

    if (footerStart == std::string::npos) {
//        std::cerr << "Footer 'END' not found or data is not complete!" << std::endl;
        return false;
    }

    size_t dataEnd = headerPos + footerStart;
    actualDataSize = dataEnd - (headerPos + 1);

    if (actualDataSize > dataSize) {
        std::cerr << "Provided data array is too small!" << std::endl;
        return false;
    }

    std::memcpy(data, &buffer[headerPos + 1], actualDataSize);

    head = headerPos + footerStart + sizeof(footer) - 1;

    return true;
    }

void Buffer::clearProcessedData() {
        if (head >= tail) {
            head = tail = 0;
            return;
        }

        size_t remainingDataSize = tail - head;

        if (remainingDataSize > 0) {
            std::memmove(&buffer[0], &buffer[head], remainingDataSize);
        }

        tail = remainingDataSize;
        head = 0;
    }

void Buffer::printBuffer() {
        std::cout << "     Buffer state: ";
        for (size_t i = 0; i < tail; ++i) {
            std::cout << buffer[i];
        }
        std::cout << std::endl;
    }
