#include"../include/ttaslock.h"
using namespace std;


TTASLock::TTASLock(int* numthread) {
    state =false ;
    size =  *numthread;// this is dummy variable just to keep all the locks constructor call of same function signature while creating a template lock object 
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
