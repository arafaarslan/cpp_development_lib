#ifndef THREAD_SAFE_DOUBLY_LINKED_LIST_H
#define THREAD_SAFE_DOUBLY_LINKED_LIST_H

#include <mutex>
#include <sstream>

template <typename T>
class ThreadSafeDoublyLinkedList {
private:
    struct Node {
        T data;
        Node* prev;
        Node* next;
        Node(const T& value) : data(value), prev(nullptr), next(nullptr) {}
    };

    Node* head;
    Node* tail;
    mutable std::mutex mtx;

public:
    ThreadSafeDoublyLinkedList() : head(nullptr), tail(nullptr) {}

    ~ThreadSafeDoublyLinkedList() {
        clear();
    }

    void push_front(const T& value) {
        std::lock_guard<std::mutex> lock(mtx);
        Node* newNode = new Node(value);
        newNode->next = head;
        if (head)
            head->prev = newNode;
        else
            tail = newNode;  // first node
        head = newNode;
    }

    void push_back(const T& value) {
        std::lock_guard<std::mutex> lock(mtx);
        Node* newNode = new Node(value);
        newNode->prev = tail;
        if (tail)
            tail->next = newNode;
        else
            head = newNode;  // first node
        tail = newNode;
    }

    bool pop_front(T& result) {
        std::lock_guard<std::mutex> lock(mtx);
        if (!head)
            return false;
        Node* temp = head;
        result = temp->data;
        head = head->next;
        if (head)
            head->prev = nullptr;
        else
            tail = nullptr;
        delete temp;
        return true;
    }

    bool pop_back(T& result) {
        std::lock_guard<std::mutex> lock(mtx);
        if (!tail)
            return false;
        Node* temp = tail;
        result = temp->data;
        tail = tail->prev;
        if (tail)
            tail->next = nullptr;
        else
            head = nullptr;
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
        tail = nullptr;
    }

    void print_forward() const {
        std::lock_guard<std::mutex> lock(mtx);
        Node* current = head;
        Logger::Debug("List (forward): ");
        std::ostringstream oss;
        while (current) {
            oss.str(""); 
            oss.clear(); 
            oss << current->data;
            Logger::Debug(oss.str());
            //std::cout << current->data << " <-> ";
            current = current->next;
        }
    }

    void print_backward() const {
        std::lock_guard<std::mutex> lock(mtx);
        Node* current = tail;
        Logger::Debug("List (backward): ");
        std::ostringstream oss;
        while (current) {
            oss.str(""); 
            oss.clear(); 
            oss << current->data;
            Logger::Debug(oss.str());
            //std::cout << current->data << " <-> ";
            current = current->prev;
        }
    }
};

#endif