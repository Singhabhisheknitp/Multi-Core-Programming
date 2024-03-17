#include<iostream>
#include<thread>
#include <vector>
#include <chrono>
#include <fstream>
#include <atomic>
#include <unistd.h>
#include <functional>
using namespace std;

class TTASLock {
    public:
    atomic<bool> state;
    int size; // dummy variable just to make all class template standard for initialising the constructor
    TTASLock(int* numthread) {
        state =false ;
        size =  *numthread;
    }

    void lock() {
        while (true) {
            while (this->state.load()) {}
            if (!this->state.exchange(true)) {
                return;
            }
        }
    }
    void unlock() {
        (this->state).store(false);
    }
};