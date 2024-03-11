#include<iostream>
#include<thread>
#include<mutex>
#include <vector>
#include <chrono>
#include <fstream>
#include <atomic>
#include <unistd.h>
using namespace std;



void cs_alock(){
    for (int j = 0; j < 100000; j++){
        n->lock();
        //CS 
    int c = 0; int d = 0; int e = 0; int f = 0;
    for (int i = 0; i < 100; i++){
        c = c+1; 
        d = d+1;
        e = e+1;
        f = f+1;
    }  
        n->unlock();
       
    }
}