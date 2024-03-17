#include<iostream>
#include<thread>
#include <vector>
#include <chrono>
#include <fstream>
#include <atomic>
#include <unistd.h>
#include <functional>
using namespace std;


class TASLock {
    public:
    atomic<bool> state;
    int size; // dummy variable just to make all class template standard for initialising the constructor

    TASLock(int* numthread) {
        state =false;
        size =  *numthread;
        }

    void lock() {
        while (this->state.exchange(true)) {}
    }

    void unlock() {
        (this->state).store(false);
    }
};
