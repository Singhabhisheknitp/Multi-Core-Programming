#include "../include/alock.h"
using namespace std;

thread_local int ALock::id;

ALock::ALock(int* numthread) {
    tail.store(0);
    size = *numthread;
    flag = new bool[size];
    for (int i = 0; i < size; i++) {
        flag[i] = (false);
    }
    flag[0] = true;
}

void ALock::init() {
    id = 0;
}

void ALock::lock() {
    init(); // to initialise thread local variable
    int slot = tail.fetch_add(1)%size;
    id = slot;
    while (!flag[slot]) {}
}

void ALock::unlock() {
    int slot = id;
    flag[slot] = false;
    flag[(slot + 1)%size] = true;
}




