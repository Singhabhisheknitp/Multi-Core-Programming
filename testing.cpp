#include <iostream>
#include <atomic>
#include <typeinfo>
#include <bitset>
#include <cstdint>
using namespace std;

// class Check{

// public:
// struct tagptr {
//     bitset<64> ptr;
//     bitset<64> tag;

//     tagptr(int* node) {
//         uintptr_t address = reinterpret_cast<uintptr_t>(node);
//         ptr = (address & 0x0000FFFFFFFFFFFF); 
//         tag = (address & 0xFFFF000000000000); 
//     }
// };

// template<typename T>
// struct Node {
//     T value;
//     Node* next;
//     Node* prev;
//     Node(T value) {
//         this->value = value;
//         this->next = nullptr;
//         this->prev = nullptr;
//     }
// };

// template<typename T>
// bool CAS(atomic<Node<T>*> current, Node<T>* expected, Node<T>* desired) {
//     tagptr desired = tagptr(desired);

//     return atomic_compare_exchange_weak(&current, &expected, desired);
// }
// };

template<typename Node>
struct tagptr {
    bitset<64> ptr;
    bitset<64> tag;

    tagptr(T* node) {
        uintptr_t address = reinterpret_cast<uintptr_t>(node);
        ptr = (address & 0x0000FFFFFFFFFFFF); 
        tag = ((address & 0xFFFF000000000000)>>48); 
    }

    T* getptr(){
        bitset<64> b = ptr; 
        uint64_t address = b.to_ullong();
        T* pointer = reinterpret_cast<T*>(address);
        return pointer;
    }

    
};

template<typename T>
struct Node {
    T value;
    tagptr next;
    Node(T value) {
        this->value = value;
        this->next = next(nullptr);
    }
};



int main()
{
    Node<int>* dummy = new Node<int>(0);
    
    cout << "Address of a: " <<(dummy.next.getptr())<< endl;
    // cout << "Address of ptr: " << (p.getptr()) << endl;
    // cout << "Address of tag: " << (p.tag) << endl;
    // cout<< "Address of a: " <<(p.ptr|(p.tag<<48))<<endl;
    // cout<< "type of p.ptr: " << typeid(p.ptr).name() << endl;

return 0;

}