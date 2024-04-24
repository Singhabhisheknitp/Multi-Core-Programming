#include <iostream>
#include <atomic>
#include <typeinfo>
using namespace std;

template<typename T>
struct tagptr {
    uintptr_t ptr;
    uintptr_t tag;
    uintptr_t whole;

    tagptr(T node) {
        uintptr_t address = reinterpret_cast<uintptr_t>(std::addressof(*node));
        ptr =  (address & 0x0000FFFFFFFFFFFF); 
        tag = ((address & 0xFFFF000000000000)>>48); 
        whole = address;
    }

    void tagadd(int n) {
        this->tag = this->tag + n;
        whole = (ptr | (this->tag << 48));
    }
    
};

template<typename T>
struct Node {
    T value;
    tagptr<Node*> next;
    Node(T value) : value(value), next(nullptr) {}
};

int main()
{   tagptr<atomic<Node<int>*>> a (new Node<int>(5));
    // tagptr<atomic<Node<int>*>> b (a);
    cout << a.whole << endl;
    // cout<< b.ptr << endl;
    a.tagadd(1);
    cout << a.whole << endl;
    // cout<< b.ptr<< endl;
    
return 0;

}