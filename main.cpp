#include"src/taslock.cpp"
#include"src/ttaslock.cpp"
#include"src/clhlock.cpp"
#include"utills.cpp"

int main(){
    TTASLock* n = new TTASLock();
    TASLock* m = new TASLock();
    // CLHLock* o = new CLHLock();

    int thread_count = 40;
    int step = 2;
    use_lock_and_write_to_csv(m, "tas.csv", "taslock", step, thread_count);
    use_lock_and_write_to_csv(n, "ttas.csv", "ttaslock", step, thread_count);
    // use_lock_and_write_to_csv(o, "clh.csv", "clhlock", step, thread_count);
    

    return 0;
}
