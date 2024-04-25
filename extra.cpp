#include <cstdint>
#include <iostream>
#include <atomic>
using namespace std;

template<typename T>
struct TaggedPtr {
    uintptr_t ptr;  // Lower 48 bits for the pointer, upper 16 bits for the tag

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

    Node(T val) : value(val) {}
};

template<typename T>
struct MSQueue {
    TaggedPtr<Node<T>> Head;
    TaggedPtr<Node<T>> Tail;

    MSQueue() {
        Node<T>* dummy = new Node<T>(0);
        TaggedPtr<Node<T>> tp(dummy);
        Head = (tp);
        Tail = (tp);
    }

    void enqueue(T value) {
       
        Node<T>* node = new Node<T>(value);
        TaggedPtr<Node<T>> tail, next;
        

        while (true) {
            // cout<<Tail.ptr<<endl;
            TaggedPtr<Node<T>> tail(Tail);
            TaggedPtr<Node<T>> next(tail.getPtr()->next);
            // cout<<"Enqueueing "<<value<<endl;
            
            if (tail.ptr == Tail.ptr) { // Check consistency
                if (next.getPtr() == nullptr) { // Tail is pointing to the last node
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
};
