#pragma once
#include <atomic>
#include <iostream>
#include"tagpointerABA.cpp"
#include"node.cpp"


template<typename T>
class TreiberStack {
public:

    std::atomic<TaggedPtr<Node<T>>> Top;
    TreiberStack() {
        Node<T>* dummy = new Node<T>(0);
        TaggedPtr<Node<T>> tp(dummy);
        Top = tp;

          for (int i = 1; i < 64000; ++i) {
            push(i);  
        }
    }
    


    void push(T value) {
        Node<T>* node = new Node<T>(value);
        TaggedPtr<Node<T>> old_top = Top.load();
        do {
            node->next = old_top;
        } while (!Top.compare_exchange_weak(old_top, node));
    }

    Node<T>* pop() {
        TaggedPtr<Node<T>> old_top = Top.load();
        TaggedPtr<Node<T>> new_top;
        do {
            if (old_top.getPtr() == nullptr) {
                return T();
            }
            new_top = old_top.getPtr()->next;
        } while (!Top.compare_exchange_weak(old_top, new_top));
        Node<T>* node = old_top.getPtr();
        return node;
    }
};


