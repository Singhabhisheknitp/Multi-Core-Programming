#include <cstdint>
#include <iostream>
#include <atomic>
#pragma once
using namespace std;

template<typename T>
struct TaggedPtr {
    uintptr_t ptr;  // full 64 bit pointer

    static constexpr uintptr_t PTR_MASK = 0x0000FFFFFFFFFFFF;
    static constexpr uintptr_t TAG_MASK = 0xFFFF000000000000;

    TaggedPtr() : ptr(0) {}

    TaggedPtr(T* p, uint16_t tag = 0) {
        set(p, tag);
    }

    T* getPtr() const {
        return reinterpret_cast<T*>(ptr & PTR_MASK);
    }

    uint16_t getTag() const {
        return static_cast<uint16_t>((ptr & TAG_MASK) >> 48);
    }

    void set(T* p, uint16_t tag) {
        uintptr_t p_int = reinterpret_cast<uintptr_t>(p);
        ptr = (p_int & PTR_MASK) | (static_cast<uintptr_t>(tag) << 48);
    }
};
