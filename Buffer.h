#include <iostream>
#include <vector>
#include <cstring>
#include <string>

using namespace std;

class Buffer {
public:
    Buffer(size_t maxSize) : buffer(maxSize), head(0), tail(0) {}

    bool pushData(const char* data, size_t size) ;

    bool processData(char& header, char* data, size_t dataSize, size_t &actualDataSize);

    void clearProcessedData();

    void printBuffer() ;

private:
    std::vector<char> buffer;
    size_t head;
    size_t tail;
};
