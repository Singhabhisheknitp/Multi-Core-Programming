The code implents a concurrent lock free stack data structure through universality consesnsus concept.
Concept used: 
1. Multiple Concurrent threads appends its method (of seq object) to a linked list through the consesus object
2. Consensus object is built using CAS(Comapre and set) atomic instruction
3. CAS atomic instruction is built using __asm__ directive in C++ and have not used standard "CMPXCHG" availabel in X86 processor.
   
