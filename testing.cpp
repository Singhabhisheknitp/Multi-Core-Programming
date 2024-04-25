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
        "lock; cmpxchgq %2, %1\n\t"  // Compare and exchange
        "sete %0\n\t"                // Set 'result' based on zero flag
        : "=a" (result),             // Output
          "+m" (*addr)               // Input/output: memory operand
        : "r" (newVal),              // Input: new value
          "a" (oldVal)               // Input: expected old value
        : "memory", "cc"             // Clobbers
    );
    return result;
}

int main() {
    Node<int>* a = new Node<int>(5); // Assume a points to Node with value 5
    Node<int>* b = a; // Assume b points to Node with value 7
    Node<int>* c = new Node<int>(8); // Assume c points to Node with value 8

    cout<<"a: "<<a<<endl;
    cout<<"b: "<<b<<endl;
    cout<<"c: "<<c<<endl;

    // Example usage: Change 'a' to point to 'c' if 'a' currently points to the same Node as 'b'
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
