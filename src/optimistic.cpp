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

    T dequeue() {
       while(true){
        Node* front = head.load();
        Node* last = tail.load();
        Node* second = front->prev; // second just next to head
        if(front == head.load()){
            if(front != last){
                if (second == nullptr) {
                    fixlist(front, last);
                    continue;    
                }else{
                    T value = second->value;
                    if(head.compare_exchange_weak(front, second)){
                        delete last;
                        return value;
                    }
                }
            }else{
                return 0;
            }

        }

      }
    }

    void fixlist(Node* front, Node* last){
       Node* curr = last;
       Node* succ;
       while(front == head.load() && curr != front){
           succ = curr->next;
           succ->prev = curr;
           curr = succ;
       }
    }


}; 