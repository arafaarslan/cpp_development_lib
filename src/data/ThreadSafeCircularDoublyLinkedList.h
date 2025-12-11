#ifndef THREAD_SAFE_CIRCULAR_DOUBLY_LINKED_LIST_H
#define THREAD_SAFE_CIRCULAR_DOUBLY_LINKED_LIST_H

#include <mutex>
#include <sstream>

template <typename T>
class ThreadSafeCircularDoublyLinkedList {
private:
    struct Node {
        T data;
        Node* next;
        Node* prev;
        Node(const T& value) : data(value), next(nullptr), prev(nullptr) {}
    };

    Node* head; // pointer to first element
    mutable std::mutex mtx;

public:
    ThreadSafeCircularDoublyLinkedList() : head(nullptr) {}

    ~ThreadSafeCircularDoublyLinkedList() {
        clear();
    }

    void push_back(const T& value) {
        std::lock_guard<std::mutex> lock(mtx);
        Node* newNode = new Node(value);

        if (!head) {
            head = newNode;
            head->next = head;
            head->prev = head;
        } else {
            Node* tail = head->prev;
            tail->next = newNode;
            newNode->prev = tail;
            newNode->next = head;
            head->prev = newNode;
        }
    }

    void push_front(const T& value) {
        std::lock_guard<std::mutex> lock(mtx);
        Node* newNode = new Node(value);

        if (!head) {
            head = newNode;
            head->next = head;
            head->prev = head;
        } else {
            Node* tail = head->prev;
            newNode->next = head;
            newNode->prev = tail;
            tail->next = newNode;
            head->prev = newNode;
            head = newNode;
        }
    }

    bool pop_front(T& result) {
        std::lock_guard<std::mutex> lock(mtx);
        if (!head)
            return false;

        result = head->data;

        if (head->next == head) {
            delete head;
            head = nullptr;
        } else {
            Node* tail = head->prev;
            Node* temp = head;
            head = head->next;
            tail->next = head;
            head->prev = tail;
            delete temp;
        }
        return true;
    }

    bool pop_back(T& result) {
        std::lock_guard<std::mutex> lock(mtx);
        if (!head)
            return false;

        Node* tail = head->prev;
        result = tail->data;

        if (tail == head) {
            delete head;
            head = nullptr;
        } else {
            Node* newTail = tail->prev;
            newTail->next = head;
            head->prev = newTail;
            delete tail;
        }
        return true;
    }

    bool find(const T& value) const {
        std::lock_guard<std::mutex> lock(mtx);
        if (!head)
            return false;

        Node* current = head;
        do {
            if (current->data == value)
                return true;
            current = current->next;
        } while (current != head);

        return false;
    }

    void clear() {
        std::lock_guard<std::mutex> lock(mtx);
        if (!head)
            return;

        Node* current = head;
        do {
            Node* temp = current;
            current = current->next;
            delete temp;
        } while (current != head);

        head = nullptr;
    }

    void print_forward() const {
        std::lock_guard<std::mutex> lock(mtx);
        if (!head) {
            Logger::Debug("List is empty");
            return;
        }

        Node* current = head;
        Logger::Debug("Circular Doubly Linked List (Forward): ");
        std::ostringstream oss;
        do {
            oss.str(""); 
            oss.clear(); 
            oss << current->data;
            Logger::Debug(oss.str());
            current = current->next;
        } while (current != head);

        Logger::Debug("(back to head)");
    }

    void print_backward() const {
        std::lock_guard<std::mutex> lock(mtx);
        if (!head) {
            Logger::Debug("List is empty");
            return;
        }

        Node* tail = head->prev;
        Node* current = tail;
        Logger::Debug("Circular Doubly Linked List (Backward): ");
        std::ostringstream oss;
        do {
            oss.str(""); 
            oss.clear(); 
            oss << current->data;
            Logger::Debug(oss.str());
            current = current->prev;
        } while (current != tail);

        Logger::Debug("(back to tail)");
    }
};

#endif