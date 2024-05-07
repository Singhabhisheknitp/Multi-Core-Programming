#pragma once
#include<iostream>
#include"tagpointerABA.cpp"
using namespace std;

template<typename T>
 struct Node {
    T value;
    TaggedPtr<Node<T>> next;
    Node(T val) : value(val), next(nullptr, 0) {}
    };
    