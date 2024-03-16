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
    TTASLock() : state(false) {}

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