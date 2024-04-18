#include<atomic>
#include<mutex>
using namespace std;

template<typename T>
class Twolockqueue {
private:
    struct Node {
        T value;
        Node* next;
        Node(T value) {
            this->value = value;
            this->next = nullptr;
        }
    };
    Node* tail;
    Node* head;
    mutex tail_lock, head_lock;


public:
    Twolockqueue() {
        Node* dummy = new Node(T());
        head = dummy;
        tail = dummy;
    }

    void enqueue(T value) {
        Node* node = new Node(value);
        tail_lock.lock();
        tail->next = node;
        tail = node;
        tail_lock.unlock();
        
        
    }

    bool dequeue() {
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
            return true;
        }

 }
}; 