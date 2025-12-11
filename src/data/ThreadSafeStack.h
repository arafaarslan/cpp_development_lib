#ifndef THREADSAFESTACK_H
#define THREADSAFESTACK_H

#include <mutex>
#include <stack>

template<typename T>
class ThreadSafeStack {
public:
    void push(T value) {
        std::lock_guard<std::mutex> lock(m_);
        s_.push(std::move(value));
    }

    bool try_pop(T& out) {
        std::lock_guard<std::mutex> lock(m_);
        if (s_.empty()) return false;
        out = std::move(s_.top());
        s_.pop();
        return true;
    }

    bool empty() const {
        std::lock_guard<std::mutex> lock(m_);
        return s_.empty();
    }

    size_t size() const {
        std::lock_guard<std::mutex> lock(m_);
        return s_.size();
    }

private:
    mutable std::mutex m_;
    std::stack<T> s_;
};

#endif