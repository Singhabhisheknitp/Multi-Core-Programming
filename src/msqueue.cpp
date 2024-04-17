#include<atomic>
// #include"hazard_pointer.h"

template<typename T>
class MSqueue {
private:
    struct Node {
        T value;
        std::atomic<Node*> next;
        Node(T value) {
            this->value = value;
            this->next = nullptr;
        }
    };
    std::atomic<Node*> tail;
    std::atomic<Node*> head;


public:
    MSqueue() {
        Node* dummy = new Node(T());
        head.store(dummy);
        tail.store(dummy);
    }

    void enqueue(T value) {
        Node* node = new Node(value);
        Node* last;
        Node* next;
        while (true) {
            last = tail.load();
            next = last->next.load();
            if (last == tail.load()) {
                if (next == nullptr) {
                    if (last->next.compare_exchange_weak(next, node)) {
                        tail.compare_exchange_weak(last, node);
                        break;
                    }
                } else {
                    tail.compare_exchange_weak(last, next);
                }
            }
        }
    }

    T dequeue() {
        while(true){
        Node* front = head.load();
        Node* last = tail.load();
        Node* next = front->next.load();
        if (front == head.load()) {
            if (front == last) {
                if (next == nullptr) {
                    return false;
                }
                else {
                    tail.compare_exchange_weak(last, next);  // if front & last same without next being null pointer,
                                                            // then just one node added halfway and tail field not updated
                                                           //hence helping other as typical lockfree pattern
                }
                
            } else {
                T value = next->value;
                if (head.compare_exchange_weak(front, next)) {
                    return value;
                }
            }
        }
    }

 }
}; 