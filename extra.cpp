#include <cstdint>
#include <iostream>
#include <atomic>
using namespace std;

template<typename T>
struct TaggedPtr {
    uintptr_t ptr;  // full 64 bit pointer

    static constexpr uintptr_t PTR_MASK = 0x0000FFFFFFFFFFFF;
    static constexpr uintptr_t TAG_MASK = 0xFFFF000000000000;

    TaggedPtr() : ptr(0) {}

    TaggedPtr(T* p, uint16_t tag = 0) {
        set(p, tag);
    }

    T* getPtr() const {
        return reinterpret_cast<T*>(ptr & PTR_MASK);
    }

    uint16_t getTag() const {
        return static_cast<uint16_t>((ptr & TAG_MASK) >> 48);
    }

    void set(T* p, uint16_t tag) {
        uintptr_t p_int = reinterpret_cast<uintptr_t>(p);
        ptr = (p_int & PTR_MASK) | (static_cast<uintptr_t>(tag) << 48);
    }
};

template<typename T>
bool cas(TaggedPtr<T>* addr, TaggedPtr<T> oldVal, TaggedPtr<T> newVal) {
    uintptr_t expected = oldVal.ptr;
    uintptr_t desired = newVal.ptr;
    return __sync_bool_compare_and_swap(&addr->ptr, expected, desired);
}


template<typename T>
struct Node {
    T value;
    TaggedPtr<Node<T>> next;

    Node(T val) : value(val), next(nullptr, 0) {}
};

template<typename T>
struct MSQueue {
    TaggedPtr<Node<T>> Head;
    TaggedPtr<Node<T>> Tail;

    MSQueue() {
        Node<T>* dummy = new Node<T>(0);
        TaggedPtr<Node<T>> tp(dummy);
        Head = tp;
        Tail = tp;
    }

    void enqueue(T value) {
       
        Node<T>* node = new Node<T>(value);
        TaggedPtr<Node<T>> tail, next;
        while (true) {
            tail= Tail;
            next = tail.getPtr()->next;  
            if (tail.ptr == Tail.ptr) { 
                if (next.getPtr() == nullptr) { 
                    if (cas(&(tail.getPtr()->next), TaggedPtr<Node<T>>(nullptr, next.getTag()), TaggedPtr<Node<T>>(node, next.getTag() + 1))) {
                        cas(&Tail, tail, TaggedPtr<Node<T>>(node, tail.getTag() + 1));
                        break;
                    }
                } else {
                    cas(&Tail, tail, TaggedPtr<Node<T>>(next.getPtr(), tail.getTag() + 1));
                }
            }
        }

        
    }
    void dequeue(){
        while(true){
            TaggedPtr<Node<T>> head = Head;
            TaggedPtr<Node<T>> tail = Tail;
            TaggedPtr<Node<T>> next = head.getPtr()->next;
            if(head.ptr == Head.ptr){
                if(head.ptr == tail.ptr){
                    if(next.getPtr() == nullptr){
                        cout << "queue empty" << endl;
                        return;
                    }
                    cas(&Tail, tail, TaggedPtr<Node<T>>(next.getPtr(), tail.getTag() + 1)); //meaning some node is added to tail.next so tail is lagging by one node swing to update that
                } else {
                    T value = next.getPtr()->value;
                    if(cas(&Head, head, TaggedPtr<Node<T>>(next.getPtr(), head.getTag() + 1))){
                        
                        return;
                    }
                }
            }
        }
    }
};
