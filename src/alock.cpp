

using namespace std;

thread_local std::atomic<int> ALock::id(0);

ALock::ALock(int* numthread) {
    tail = new std::atomic<int>(0);
    size = *numthread;
    flag = new std::atomic<bool>[size]();
    flag[0].store(true);
}

void ALock::init() {
    id.store(0);
}

void ALock::lock() {
    init();
    int slot = tail->fetch_add(1);
    id.store(slot);
    while (!flag[slot].load()) {}
}

void ALock::unlock() {
    int slot = id.load();
    flag[slot].store(false);
    flag[(slot + 1) % size].store(true);
}




