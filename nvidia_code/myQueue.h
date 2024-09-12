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
    variant <Types...> *array;

    public:
    myQueue(unsigned capacity)
        : capacity(capacity), front(0), size(0), rear(capacity - 1) {
        array = new std::variant<Types...>[this->capacity];
    }

    ~myQueue() {
        delete[] array;
    }

    bool isFull() const {
        return (size == capacity);
    }

    bool isEmpty() const {
        return (size == 0);
    }

    void push(const variant<Types...>& item) {
        if (isFull()) {
            cerr << "Queue is full" << endl;
            return;
        }

        rear = (rear + 1) % capacity;
        array[rear] = item;
        size = size + 1;
    }

    variant <Types...> pop() {
        if (isEmpty()) {
            cerr << "Queue is empty" << endl;
            return ;
        }

        auto item = array[front];  
        front = (front + 1) % capacity;
        size = size - 1;
        return item;
    }

    int getSize() const {
        return size;
    }
};

#endif // MYQUEUE_H
