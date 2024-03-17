#include<iostream>
#include<thread>
#include <vector>
#include <chrono>
#include <fstream>
#include <atomic>
#include <unistd.h>
#include <functional>
using namespace std;

class Tnode1{
public:
    atomic<bool> wait;
    Tnode1* next;
    Tnode1(){
        wait = false;
        next = nullptr;
    }
};


class MCSLock {
    public:
    atomic<Tnode1*> tail;
    static thread_local atomic<Tnode1*> myNode;
    int size; // dummy variable just to make all class template standard for initialising the constructor

    MCSLock(int* numthread) {
        atomic<Tnode1*> tail(nullptr);
        myNode.store(new Tnode1());
        size =  *numthread;
        }

    void lock() {
        Tnode1* cnode = myNode.load();
        Tnode1* pnode = tail.exchange(cnode);
        if (pnode != nullptr) {
        cnode->wait = true;
        pnode->next = cnode;
        while(cnode->wait){}
        } 
    }

    void unlock() {
        Tnode1* cnode = myNode.load();
        if (cnode->next == nullptr) {
        if (tail.compare_exchange_strong(cnode, nullptr)) 
        {return;}
        while(cnode->next == nullptr){};
        cnode->next->wait = false;
        cnode->next = nullptr;
        }

    }
};

thread_local atomic<Tnode1*> MCSLock::myNode;


