#include <iostream>
#include <atomic>
#include <cstdint>
using namespace std;

template<typename T>
struct Node {
    T value;
    Node<T>* next;

    Node(T val) : value(val), next(nullptr) {}
};

template<typename T>
bool cas(Node<T>* volatile* addr, Node<T>* oldVal, Node<T>* newVal) {
    bool result;
    __asm__ __volatile__ (
        "lock; cmpxchgq %2, %1\n\t"  
        "sete %0\n\t"                
        : "=a" (result),             
          "+m" (*addr)               
        : "r" (newVal),              
          "a" (oldVal)               
        : "memory", "cc"             
    );
    return result;
}

int main() {
    Node<int>* a = new Node<int>(5); 
    Node<int>* b = a; 
    Node<int>* c = new Node<int>(8); 

    cout<<"a: "<<a<<endl;
    cout<<"b: "<<b<<endl;
    cout<<"c: "<<c<<endl;

   
    if (cas(&a, b, c)) {
        std::cout << "CAS succeeded, a now points to new Node with value: " << a->value << std::endl;
    } else {
        std::cout << "CAS failed, a still points to Node with value: " << a->value << std::endl;
    }


    cout<<"a: "<<a<<endl;
    cout<<"b: "<<b<<endl;
    cout<<"c: "<<c<<endl;

    return 0;
}