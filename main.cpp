#include"src/taslock.cpp"
#include"src/ttaslock.cpp"
#include"src/clhlock.cpp"
#include"src/alock.cpp"
#include"src/mcslock.cpp"
#include"utills.cpp"


int main(){
    int thread_count = 2;
    int step = 1;
    // use_lock_and_write_to_csv<TASLock>( "tas.csv", "taslock", step, thread_count);
    // use_lock_and_write_to_csv<TTASLock>("ttas.csv", "ttaslock", step, thread_count);
    // use_lock_and_write_to_csv<ALock>("alock.csv", "alock", step, thread_count);
    // use_lock_and_write_to_csv<CLHLock>("clh.csv", "clhlock", step, thread_count);
    use_lock_and_write_to_csv<MCSLock>("mcs.csv", "mcslock", step, thread_count);
    return 0;
}
