#ifndef SAFEQUEUE_H
#define SAFEQUEUE_H

#include <queue>
#include <variant>
#include <pthread.h>
#include <stdexcept>


template<typename... Types>
class SafeQueue {
private:
    std::queue<std::variant<Types...>> queue;  // Thay đổi từ myQueue sang std::queue
    pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;  // Khởi tạo mutex
    pthread_cond_t cv = PTHREAD_COND_INITIALIZER;      // Khởi tạo biến điều kiện

public:

    // Thêm phần tử vào queue
    void enqueue(const std::variant<Types...>& value) {
        pthread_mutex_lock(&mtx);
        queue.push(value);
        pthread_cond_signal(&cv);
        pthread_mutex_unlock(&mtx);
    }

    // Lấy phần tử từ queue
    std::variant<Types...> dequeue() {
        pthread_mutex_lock(&mtx);
        while (queue.empty()) {
            pthread_cond_wait(&cv, &mtx);
        }
        auto value = queue.front();
        queue.pop();
        pthread_mutex_unlock(&mtx);
        return value;
    }

    // Kiểm tra xem queue có rỗng không
    bool empty() {
        pthread_mutex_lock(&mtx);
        bool isEmpty = queue.empty();
        pthread_mutex_unlock(&mtx);
        return isEmpty;
    }

    // Lấy kích thước của queue
    size_t size() {
        pthread_mutex_lock(&mtx);
        size_t size = queue.size();
        pthread_mutex_unlock(&mtx);
        return size;
    }
};

#endif // SAFEQUEUE_H
