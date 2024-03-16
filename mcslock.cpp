#include<iostream>
#include<thread>
#include <vector>
#include <chrono>
#include <fstream>
#include <atomic>
#include <unistd.h>
#include <functional>
using namespace std;

class Tnode{
public:
    atomic<bool> wait;
    Tnode* next;
    Tnode(){
        wait = false;
        next = nullptr;
    }
};


class MCSLock {
    public:
    atomic<Tnode*> tail;
    static thread_local atomic<Tnode*> myNode;

    MCSLock() {
        atomic<Tnode*> tail(nullptr);
        myNode.store(new Tnode());
        }

    void lock() {
        Tnode* cnode = myNode.load();
        Tnode* pnode = tail.exchange(cnode);
        if (pnode != nullptr) {
        cnode->wait = true;
        pnode->next = cnode;
        while(cnode->wait){}
        } 
    }

    void unlock() {
        Tnode* cnode = myNode.load();
        if (cnode->next == nullptr) {
        if (tail.compare_exchange_strong(cnode, nullptr)) 
        {return;}
        while(cnode->next == nullptr){};
        cnode->next->wait = false;
        cnode->next = nullptr;
        }

    }
};

thread_local atomic<Tnode*> MCSLock::myNode;


