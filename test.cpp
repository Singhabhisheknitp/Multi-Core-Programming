// learnings: 1. anything inside function is local to each threads
               // (each thread builds its stack so even if declared on heap it is local to that thread because 
               // each thread calls its ow on malloc so separate memory is allocated for each thread on heap)
//            2. anything inside class is shared among threads (stack or heap)
//            3. anything declared as static thread_local inside class is local among threads (stack or heap)





#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <atomic>
using namespace std;
mutex m;
mutex m2;
class check {
public:
    
    atomic<int> tail1;  // STACK + CLASS
    static thread_local atomic<int> tail3 ;  //STACK + THREAD_LOCAL + CLASS

    atomic<int>* tail2; // HEAP + CLASS
    static thread_local atomic<int>* tail4 ; //HEAP + THREAD_LOCAL + CLASS
        
    check() {
        tail1.store(0);   
        tail2 = new atomic<int>(0);
    }

       
        void c() {
            tail3 = 0;   
            tail4 = new atomic<int>(0);        
        }

            void inc() {
                m2.lock();
                // tail2.fetch_add(1);
                cout<<tail2<<endl;
                m2.unlock();
            }
        void print(){
            
            c();
            // inc(); 
            m2.lock();
            tail4->fetch_add(1); 
            cout<<*tail4<<endl;
            m2.unlock();
        }

        void func(){
            int* a =  new int(5);
            int* b =  new int(7);
            m2.lock();   
            cout<<a<<endl;
            m2.unlock();
           
        }
};









thread_local atomic<int> check::tail3;
thread_local atomic<int>* check::tail4;


   

int main(){
    check c;
    vector <thread> threads;
    for(int i = 0; i < 10; i++){
        threads.push_back(thread(&check::func, &c));
    }
    for(auto& thread : threads) {thread.join();}
    

    return 0;
}
