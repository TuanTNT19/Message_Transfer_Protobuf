#include <iostream>
#include <vector>
#include <cstring>
#include <string>

using namespace std;

class Buffer {
public:
    Buffer(size_t maxSize) : buffer(maxSize), head(0), tail(0) {}

    bool pushData(const char* data, size_t size) {
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

    bool processData(char& header, char* data, size_t dataSize, size_t &actualDataSize) {
        if (head >= tail) {
            std::cerr << "Buffer is empty!" << std::endl;
            return false;
        }

        // Tìm header trong buffer ('A' hoặc 'B')
        size_t headerPos = head;
        while (headerPos < tail && buffer[headerPos] != 'A' && buffer[headerPos] != 'B') {
            headerPos++;
        }

        if (headerPos >= tail) {
            std::cerr << "Header 'A' or 'B' not found!" << std::endl;
            return false;
        }

        header = buffer[headerPos];

        // Tìm footer "END" trong buffer sau header
        const char footer[] = "END";
        size_t footerStart = std::string(&buffer[headerPos], tail - headerPos).find(footer);

        if (footerStart == std::string::npos) {
            std::cerr << "      Footer 'END' not found or data is not complete!" << std::endl;
            return false;
        }

        size_t dataEnd = headerPos + footerStart;
        actualDataSize = dataEnd - (headerPos + 1);

        if (actualDataSize > dataSize) {
            std::cerr << "Provided data array is too small!" << std::endl;
            return false;
        }

        std::memcpy(data, &buffer[headerPos + 1], actualDataSize);
        data[actualDataSize] = '\0'; 

        head = headerPos + footerStart + sizeof(footer) - 1;

        return true;
    }

    void clearProcessedData() {
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

    void printBuffer()  {
        std::cout << "     Buffer state: ";
        for (size_t i = 0; i < tail; ++i) {
            std::cout << buffer[i];
        }
        std::cout << std::endl;
    }

private:
    std::vector<char> buffer;
    size_t head;
    size_t tail;
};

int main() {
    Buffer buff(64);  
    char header;
    char data[32] = {0};  

    char first[] = "ATuanENDAHuoBgENDA";
    char second[] = "HuAnhPTENDATuaATNEN";
    char third[] = "DBIreneBENDATzuAy";
    char ford[] = "uENDBTUANHUONGEND";

    buff.pushData(first, strlen(first));
    if (buff.processData(header, data, sizeof(data) - 1)) {
        cout << "HEADER: " << header << ". Data: " << data << endl;
    } else {
        cout << "Failed to process data" << endl;
    }

    while(buff.processData(header, data, sizeof(data) - 1)){
        cout << "HEADER: " << header << ". Data: " << data << endl;
    }

    buff.clearProcessedData();
    buff.printBuffer();  

    buff.pushData(second, strlen(second));
    while(buff.processData(header, data, sizeof(data) - 1)){
        cout << "HEADER: " << header << ". Data: " << data << endl;
    }

    buff.clearProcessedData();
    buff.printBuffer();  

    buff.pushData(third, strlen(third));
    while(buff.processData(header, data, sizeof(data) - 1)){
        cout << "HEADER: " << header << ". Data: " << data << endl;
    }

    buff.clearProcessedData();
    buff.printBuffer();  

    buff.pushData(ford, strlen(ford));
    while(buff.processData(header, data, sizeof(data) - 1)){
        cout << "HEADER: " << header << ". Data: " << data << endl;
    }

    buff.clearProcessedData();
    buff.printBuffer();  

    return 0;
}
