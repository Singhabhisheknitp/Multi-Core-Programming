#include<atomic>
using namespace std;


template<typename T>
class   OptimisticQueue {
private:
    struct Node {
        T value;
        Node* next;
        Node* prev;
        Node(T value) {
            this->value = value;
            this->next = nullptr;
            this->prev = nullptr;
        }
    };
    std::atomic<Node*> tail;
    std::atomic<Node*> head;


public:
    OptimisticQueue() {
        Node* dummy = new Node(T());
        head.store(dummy);
        tail.store(dummy);
    }

    void enqueue(T value) {
        Node* node = new Node(value);
        Node* last;
        while (true) {
            last = tail.load();
            node->next = last;
            if (tail.compare_exchange_weak(last, node)) {
                last->prev = node;
                return;
            }
        }
          
    }

}; 