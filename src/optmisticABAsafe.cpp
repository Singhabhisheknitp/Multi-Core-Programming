#pragma once
#include <cstdint>
#include <iostream>
#include <atomic>
using namespace std;
#include"tagpointerABA.cpp"
#include"node.cpp"

template<typename T>
bool cas(TaggedPtr<T>* addr, TaggedPtr<T> oldVal, TaggedPtr<T> newVal) {
    uintptr_t expected = oldVal.ptr;
    uintptr_t desired = newVal.ptr;
    return __sync_bool_compare_and_swap(&addr->ptr, expected, desired);
}


template<typename T>
class OptimisticQueueABA {
private:
    struct Node {
        T value;
        TaggedPtr<Node> next;
        TaggedPtr<Node> prev;
        Node(T value) : value(value), next(nullptr), prev(nullptr) {}
    };

  

public:
    TaggedPtr<Node> Head;
    TaggedPtr<Node> Tail;
    OptimisticQueueABA() {
        Node* dummy = new Node(0);
        TaggedPtr<Node> tp(dummy);
        Head =(tp);
        Tail =(tp);
    }

    void enqueue(T value) {
        Node* node = new Node(value);
        TaggedPtr<Node> tail;
        while (true) {
            tail = Tail;
            node->next = TaggedPtr<Node>(tail.getPtr(), tail.getTag() + 1);
            if (cas(&Tail, tail, TaggedPtr<Node>(node, tail.getTag() + 1))) {
                tail.getPtr()->prev = TaggedPtr<Node>(node, tail.getTag() + 1);
                return;
            }
        }
    }

    T dequeue() {

        TaggedPtr<Node> head; 
        TaggedPtr<Node> tail;
        TaggedPtr<Node> firstNodeprev;
        while (true) {
                head = Head;
                tail = Tail;
                firstNodeprev = head.getPtr()->prev;
                if (head.ptr == Head.ptr) {
                if (tail.ptr != head.ptr) {
                    if (firstNodeprev.getTag() != head.getTag()) {
                        fixlist(head, tail);
                        continue;
                    } else {
                        T value = firstNodeprev.getPtr()->value;
                        if (cas(&Head, head, TaggedPtr<Node>(firstNodeprev.getPtr(), head.getTag() + 1))) {
                            // delete head.getPtr();
                            return value;
                        }
                    }
                } else {
                    return T();
                }
            }
        }
    }

    void fixlist(TaggedPtr<Node> head, TaggedPtr<Node> tail) {
        TaggedPtr<Node> curr;
        TaggedPtr<Node> succ;
        curr = tail;
        while (head.ptr == Head.ptr && curr.ptr != head.ptr) {
            succ = curr.getPtr()->next;
            succ.getPtr()->prev = TaggedPtr<Node>(curr.getPtr(), curr.getTag() - 1);
            curr = TaggedPtr<Node>(succ.getPtr(), curr.getTag() - 1);
        }
    }
};