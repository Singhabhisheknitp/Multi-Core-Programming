#include<atomic>
#include<mutex>
using namespace std;

template<typename T>
class Twolockqueue {
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
    mutex tail_lock, head_lock;


public:
    Twolockqueue() {
        Node* dummy = new Node(T());
        head.store(dummy);
        tail.store(dummy);
    }

    void enqueue(T value) {
        Node* node = new Node(value);
        tail_lock.lock();
        tail.load()->next = node;
        tail = node;
        tail_lock.unlock();
        
    }

    T dequeue() {
        head_lock.lock();
        Node* front = head;
        Node* next = front->next;
        if (next == nullptr) {
            head_lock.unlock();
            return false;
        }
        else {
            T value = next->value;
            head = next;
            head_lock.unlock();
            return value;
        }

 }
}; 