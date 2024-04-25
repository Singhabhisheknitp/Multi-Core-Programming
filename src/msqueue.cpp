#include<atomic>
#include<bitset>



template<typename T>
class MSqueue {
private:
    struct Node {
        T value;
        tagptr<std::atomic<Node*>> next;
        Node(T value) {
            this->value = value;
            this->next(nullptr);
        }
    };
    tagptr<std::atomic<Node*>> Tail;
    tagptr<std::atomic<Node*>> Head;


public:
    MSqueue() {
        Node* dummy = new Node(T());
        Head = tagptr<std::atomic<Node*>>(dummy);
        Tail = tagptr<std::atomic<Node*>>(dummy);
    }

    template<typename U>
    U wrap(uintptr_t ptr, uintptr_t tag){
        uintptr_t address = reinterpret_cast<uintptr_t>(ptr);
        address = address | (tag << 48);
        return reinterpret_cast<U>(address);
    }

    void enqueue(T value) {
        Node* node = new Node(value);
        while (true) {
        tagptr<Node*> tail(Tail.lptr);
        tagptr<Node*> tailnext((tail.ptr->next).lptr);
            if (tail == Tail) {
                if (tailnext.ptr == nullptr) {
                    if (Tail.ptr->next.compare_exchange_weak(tailnext, wrap<Node*>(node, tailnext.tag + 1))) {
                        Tail.compare_exchange_weak(tail, wrap<Node*>(node, tail.tag + 1));
                        break;
                    }
                } else {
                    Tail.lptr.compare_exchange_weak(tail.lptr, wrap<Node*>(tailnext.ptr, tail.tag + 1));
                }
            }
        }
    }

    T dequeue() {
        while(true){
        Node* front = Head.load();
        Node* lastnode = Tail.load();
        Node* next = front->next.load();
        if (front == Head.load()) {
            if (front == lastnode) {
                if (next == nullptr) {
                    return false;
                }
                else {
                    Tail.compare_exchange_weak(lastnode, next);  // if front & lastnode same without next being null pointer,
                                                            // then just one node added halfway and Tail field not updated
                                                           //hence helping other as typical lockfree pattern
                }
                
            } else {
                T value = next->value;
                if (Head.compare_exchange_weak(front, next)) {
                    return value;
                }
            }
        }
    }

 }
}; 