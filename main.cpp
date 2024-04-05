#include"include/taslock.h"
#include"include/ttaslock.h"
#include"include/clhlock.h"
#include"include/alock.h"
#include"include/mcslock.h"
#include"utills.cpp"


int main(){
    int thread_count = 10;
    int step = 1;

    use_lock_and_write_to_csv<TASLock>( "tas.csv", "taslock", step, thread_count);
    use_lock_and_write_to_csv<TTASLock>("ttas.csv", "ttaslock", step, thread_count);
    use_lock_and_write_to_csv<CLHLock>("clh.csv", "clhlock", step, thread_count);
    use_lock_and_write_to_csv<ALock>("alock.csv", "alock", step, thread_count);
    use_lock_and_write_to_csv<MCSLock>("mcs.csv", "mcslock", step, thread_count);
    return 0;
}

