#include "../include/alock.h"
using namespace std;

thread_local atomic<int> ALock::id;

ALock::ALock(int* numthread) {
    tail.store(0);
    size = *numthread;
    flag = new atomic<bool>[size];
    for (int i = 0; i < size; i++) {
        flag[i].store(false);
    }
    flag[0].store(true);
}

void ALock::init() {
    id.store(0);
    // cout<<id.load()<<endl;
}

void ALock::lock() {
    init(); // to initialise thread local variable
    int slot = tail.fetch_add(1);
    cout<<tail<<endl;
    id.store(slot);
    while (!flag[slot].load()) {}
}

void ALock::unlock() {
    int slot = id.load();
    flag[slot].store(false);
    flag[(slot + 1)].store(true);
}




