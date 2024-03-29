#include"../include/ttaslock.h"
using namespace std;


TTASLock::TTASLock(int* numthread) {
    state =false ;
    size =  *numthread;
}

void TTASLock::lock() {
    while (true) {
        while (state.load()) {}
        if (!state.exchange(true)) {
            return;
        }
    }
}

void TTASLock:: unlock() {
    (state).store(false);
}
