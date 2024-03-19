#include<iostream>
#include<thread>
#include<mutex>
#include <vector>
#include <chrono>
#include <fstream>
#include <atomic>
#include <unistd.h>
using namespace std;
class ALock {
    public:
    static thread_local atomic<int> id;  // id variable local to each thread but global across the object methods as needed
    atomic<int>* tail;
    bool* flag;
    int size;
    

    ALock(int* numthread) {
        size = *numthread;
        tail = new atomic<int>(0);
        flag = new bool[size]();
        flag[0] = true;
    }

    void init() {
        id.store(0);
    }
    void lock() {
        init();
        int slot = tail->fetch_add(1);
        id.store(slot);
        while (!flag[slot]) {}
    }

    void unlock() {
        int slot = id.load();
        flag[slot] = false;
        flag[(slot + 1)] = true;
        }       
};

thread_local atomic<int> ALock::id(0);




