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
    static  Tnode1* sentinal;
    static thread_local atomic<Tnode1*> myNode;
    int size; // dummy variable just to make all class template standard for initialising the constructor

    MCSLock(int* numthread) {
        sentinal= new Tnode1();
        tail.store(sentinal);
        size =  *numthread;
        }

    void init(){
        myNode.store(new Tnode1());
    }

    void lock() {
        init();
        Tnode1* cnode = myNode.load();
        // // put a memory fence here below
        // cout<<cnode<<endl;
        // cout<<tail<<endl;
        
        Tnode1* pnode = tail.exchange(cnode);
        // cout<<tail<<endl;
        // cout<<pnode<<endl;
        if (pnode !=  sentinal) {
        // cout<<"check1"<<endl;
        cnode->wait = true;
        // cout<<"check2"<<endl;
        // cout<<pnode<<"after tail exchnage"<<endl;
        pnode->next = cnode;
        // cout<<"check3"<<endl;
        while(cnode->wait){}
        // cout<<"lock acquired"<<endl;
        } 
    }

    void unlock() {
        Tnode1* cnode = myNode.load();
        cout<<cnode<<endl;
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
Tnode1* MCSLock::sentinal;


