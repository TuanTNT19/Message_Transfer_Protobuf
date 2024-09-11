#ifndef MYQUEUE_H
#define MYQUEUE_H

#include <iostream>
#include <stdexcept>
#include <variant>

using namespace std;

template <typename... Types>
class myQueue {
private:
    int front, rear, size;
    unsigned capacity;
    std::variant<Types...>* array;

public:
    // Constructor để khởi tạo Queue với kích thước cho trước
    myQueue(unsigned capacity)
        : capacity(capacity), front(0), size(0), rear(capacity - 1) {
        array = new std::variant<Types...>[this->capacity];
    }

    // Destructor để giải phóng bộ nhớ khi đối tượng Queue bị hủy
    ~myQueue() {
        delete[] array;
    }

    // Kiểm tra Queue có đầy hay không
    bool isFull() const {
        return (size == capacity);
    }

    // Kiểm tra Queue có rỗng hay không
    bool isEmpty() const {
        return (size == 0);
    }

    // Thêm phần tử vào đầu Queue ( cuối mang)
    void push(const std::variant<Types...>& item) {
        if (isFull()) {
            throw overflow_error("Queue is full");
        }
        rear = (rear + 1) % capacity;
        array[rear] = item;
        size = size + 1;
    }

    // Lấy phần tử từ cuối queue ( đầu mảng)
    std::variant<Types...> pop() {
        if (isEmpty()) {
            throw out_of_range("Queue is empty");
        }
        std::variant<Types...> item = array[front];
        front = (front + 1) % capacity;
        size = size - 1;
        return item;
    }

    // Lấy kích thước của Queue
    int getSize() const {
        return size;
    }
};

#endif // MYQUEUE_H