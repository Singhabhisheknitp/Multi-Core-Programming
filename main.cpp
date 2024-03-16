#include"taslock.cpp"
#include"ttaslock.cpp"
#include"utills.cpp"

int main(){
    TTASLock* n = new TTASLock();
    TASLock* m = new TASLock();

    int thread_count = 40;
    int step = 4;
    use_lock_and_write_to_csv(m, "tas.csv", "taslock", step, thread_count);
    use_lock_and_write_to_csv(n, "ttas.csv", "ttaslock", step, thread_count);
    

    return 0;
}
