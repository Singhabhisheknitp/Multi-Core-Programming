#include<iostream>
#include<thread>
#include<stack>
#include<functional>
#include<mutex>
using namespace std;
mutex mtx;
using INVOC = std::function<void(std::stack<int>&)>;
class Node;
class Consensus{
    public:
    int expected; Node* winner;
    Consensus(){expected = 0;}

    Node* decide(Node* prefer, int i){
        Node* propsed_array[6];
        thread_local int ID = i;
        propsed_array[ID] = prefer;
        int value = 0; 
        int result;

        __asm__ __volatile__ (
            "movl %2, %%eax;"         
            "lock cmpxchgl %3, %1;"   
            "movl %%eax, %0;"         
            : "=r" (result), "+m" (value)
            : "r" (expected), "r" (ID)
            : "eax", "memory"
        );

        if (result == expected)

        {    
         return propsed_array[ID];

         }
        else{ return propsed_array[value];}
    }
};

class Node{
    public:
    int seq;
    Node *next;
    INVOC invoc;
    Consensus decidenext;

    Node(INVOC invoc){
        this->invoc = invoc;
        this->seq = 0;
        this->next = NULL;
        this->decidenext = Consensus(); } 

    Node(){
        this->seq = 0;
        this->next = NULL;
        this->invoc =  NULL;
        this->decidenext = Consensus();
    }

    static Node* last_node(Node* head_ref[6]){
        int n = 6;
        Node* edge = head_ref[0];
        for(int i = 0; i < n; i++){
            if(head_ref[i]->seq > edge->seq) {
                edge = head_ref[i];}
        }
        return edge;}

};

class LFuniversal{
    public:
   
    Node* head_ref[6];
    Node* tail;
    stack<int> Cstack;
  
    
    
    

    LFuniversal(){
       tail = new Node();
       tail->seq = 1;
       for(int i = 0; i < 6; i++){
           head_ref[i] = tail; }}


    void* apply(INVOC invoc, int k){
        thread_local int i = k; 
        thread_local Node* prefer = new Node(invoc);
        while (prefer->seq == 0){
        Node* before = Node::last_node(head_ref);
        Node* after = before->decidenext.decide(prefer, i);
        before->next = after;
        after->seq = before->seq + 1;
        head_ref[i] = after;
           
        } 
       
       
        return 0;}

};

int main() {    
    LFuniversal lfun = LFuniversal();
    // standard invocations
    INVOC pushOp1 = [](std::stack<int>& stack) { stack.push(1);};
    INVOC pushOp2 = [](std::stack<int>& stack) { stack.push(2);};
    INVOC pushOp3 = [](std::stack<int>& stack) { stack.push(3);};
    INVOC pushOp4 = [](std::stack<int>& stack) { stack.push(4);};
    INVOC pushOp5 = [](std::stack<int>& stack) { stack.push(5);};
    INVOC popOp1 = [](std::stack<int>& stack) { if (!stack.empty()) {stack.pop();} };
    
   
    thread t1([&lfun, &pushOp1]() {lfun.apply(pushOp1, 0);});
    thread t2([&lfun, &pushOp2]() {lfun.apply(pushOp2, 1);});
    thread t6([&lfun, &popOp1]() {lfun.apply(popOp1,   2);});
    thread t3([&lfun, &pushOp3]() {lfun.apply(pushOp3, 3);});
    thread t4([&lfun, &pushOp4]() {lfun.apply(pushOp4, 4);});
    thread t5([&lfun, &pushOp5]() {lfun.apply(pushOp5, 5);});
    

    // join the thread
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    t6.join();
   //concurrent stack
    
    Node* current = lfun.tail->next;
    while(current != NULL){
    current->invoc(lfun.Cstack);
    current = current->next;
    }

    stack<int> temp = lfun.Cstack; 
    while (!temp.empty()) {
    std::cout << temp.top() << " ";
    temp.pop();
    }
    
    return 0;
}


  
