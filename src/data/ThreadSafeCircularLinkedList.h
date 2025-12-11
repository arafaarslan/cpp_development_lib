#ifndef THREAD_SAFE_CIRCULAR_LINKED_LIST_H
#define THREAD_SAFE_CIRCULAR_LINKED_LIST_H
#include <mutex>
#include <sstream>

template <typename T>
class ThreadSafeCircularList {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& value) : data(value), next(nullptr) {}
    };

    Node* tail;  // points to the last node (tail->next = head)
    mutable std::mutex mtx;

public:
    ThreadSafeCircularList() : tail(nullptr) {}

    ~ThreadSafeCircularList() {
        clear();
    }

    // Add element to the end (after tail)
    void push_back(const T& value) {
        std::lock_guard<std::mutex> lock(mtx);
        Node* newNode = new Node(value);
        if (!tail) {
            tail = newNode;
            tail->next = tail;  // circular link to itself
        } else {
            newNode->next = tail->next;  // head
            tail->next = newNode;
            tail = newNode;
        }
    }

    // Add element to the front (before head)
    void push_front(const T& value) {
        std::lock_guard<std::mutex> lock(mtx);
        Node* newNode = new Node(value);
        if (!tail) {
            tail = newNode;
            tail->next = tail;
        } else {
            newNode->next = tail->next;
            tail->next = newNode;
        }
    }

    // Remove from front (head)
    bool pop_front(T& result) {
        std::lock_guard<std::mutex> lock(mtx);
        if (!tail)
            return false;

        Node* head = tail->next;
        result = head->data;

        if (head == tail) {
            delete head;
            tail = nullptr;
        } else {
            tail->next = head->next;
            delete head;
        }
        return true;
    }

    // Check if list contains value
    bool find(const T& value) const {
        std::lock_guard<std::mutex> lock(mtx);
        if (!tail)
            return false;

        Node* current = tail->next;
        do {
            if (current->data == value)
                return true;
            current = current->next;
        } while (current != tail->next);
        return false;
    }

    void clear() {
        std::lock_guard<std::mutex> lock(mtx);
        if (!tail)
            return;

        Node* head = tail->next;
        Node* current = head;
        do {
            Node* temp = current;
            current = current->next;
            delete temp;
        } while (current != head);

        tail = nullptr;
    }

    void print() const {
        std::lock_guard<std::mutex> lock(mtx);
        if (!tail) {
            Logger::Debug("List is empty");
            return;
        }

        Node* current = tail->next;  // head
        Logger::Debug("Circular List: ");
        std::ostringstream oss;
        do {
            oss.str(""); 
            oss.clear(); 
            oss << current->data;
            Logger::Debug(oss.str());
            current = current->next;
        } while (current != tail->next);
        
        Logger::Debug("(back to head)");
    }

};

#endif