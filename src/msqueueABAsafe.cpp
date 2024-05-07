#pragma once
#include <cstdint>
#include <iostream>
#include <atomic>
using namespace std;
#include"tagpointerABA.cpp"
#include"node.cpp"
// #include"src/trieberstack.cpp"


template<typename T>
bool CAS(TaggedPtr<T>* addr, TaggedPtr<T> oldVal, TaggedPtr<T> newVal) {
    uintptr_t expected = oldVal.ptr;
    uintptr_t desired = newVal.ptr;
    return __sync_bool_compare_and_swap(&addr->ptr, expected, desired);
}



template<typename T>
struct MSQueueABA {
    TaggedPtr<Node<T>> Head;
    TaggedPtr<Node<T>> Tail;
    // TreiberStack<int> memorypool;

    MSQueueABA() {
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
                    if (CAS(&(tail.getPtr()->next), TaggedPtr<Node<T>>(nullptr, next.getTag()), TaggedPtr<Node<T>>(node, next.getTag() + 1))) {
                        CAS(&Tail, tail, TaggedPtr<Node<T>>(node, tail.getTag() + 1));
                        break;
                    }
                } else {
                    CAS(&Tail, tail, TaggedPtr<Node<T>>(next.getPtr(), tail.getTag() + 1));
                }
            }
        }

        
    }
    void dequeue(){
        TaggedPtr<Node<T>> head;
        while(true){
            TaggedPtr<Node<T>> head = Head;
            TaggedPtr<Node<T>> tail = Tail;
            TaggedPtr<Node<T>> next = head.getPtr()->next;
            if(head.ptr == Head.ptr){
                if(head.ptr == tail.ptr){
                    if(next.getPtr() == nullptr){
                        return;
                    }
                    CAS(&Tail, tail, TaggedPtr<Node<T>>(next.getPtr(), tail.getTag() + 1)); //meaning some node is added to tail.next so tail is lagging by one node swing to update that
                } else {
                    T value = next.getPtr()->value;
                    if(CAS(&Head, head, TaggedPtr<Node<T>>(next.getPtr(), head.getTag() + 1))){
                        
                        return;
                    }
                }
            }
        }
        // (head.getPtr());
    }
};
