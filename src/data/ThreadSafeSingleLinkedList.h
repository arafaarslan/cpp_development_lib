#ifndef THREAD_SAFE_SINGLY_LINKED_LIST_H
#define THREAD_SAFE_SINGLY_LINKED_LIST_H

#include <mutex>
#include <sstream>

template <typename T>
class ThreadSafeSinglyLinkedList {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& value) : data(value), next(nullptr) {}
    };

    Node* head;
    mutable std::mutex mtx;

public:
    ThreadSafeSinglyLinkedList() : head(nullptr) {}

    ~ThreadSafeSinglyLinkedList() {
        clear();
    }

    void push_front(const T& value) {
        std::lock_guard<std::mutex> lock(mtx);
        Node* newNode = new Node(value);
        newNode->next = head;
        head = newNode;
    }

    bool pop_front(T& result) {
        std::lock_guard<std::mutex> lock(mtx);
        if (!head)
            return false;

        Node* temp = head;
        result = head->data;
        head = head->next;
        delete temp;
        return true;
    }

    bool find(const T& value) const {
        std::lock_guard<std::mutex> lock(mtx);
        Node* current = head;
        while (current) {
            if (current->data == value)
                return true;
            current = current->next;
        }
        return false;
    }

    void clear() {
        std::lock_guard<std::mutex> lock(mtx);
        Node* current = head;
        while (current) {
            Node* temp = current;
            current = current->next;
            delete temp;
        }
        head = nullptr;
    }

    void print() const {
        std::lock_guard<std::mutex> lock(mtx);
        Node* current = head;
        std::ostringstream oss;
        //std::cout << "List: ";
        while (current) {
            oss.str(""); 
            oss.clear(); 
            oss << current->data;
            Logger::Debug(oss.str());
            current = current->next;

        }
    }
};

#endif