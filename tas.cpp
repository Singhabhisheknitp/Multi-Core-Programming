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
    TASLock() : state(false) {}

    void lock() {
        while (this->state.exchange(true)) {}
    }

    void unlock() {
        (this->state).store(false);
    }
};
