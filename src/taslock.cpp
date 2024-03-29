#include "../include/taslock.h"

TASLock::TASLock(int* numthread) {
    state = false;
    size =  *numthread;
}

void TASLock::lock() {
    while (state.exchange(true)) {}
}

void TASLock::unlock() {
    state.store(false);
}
