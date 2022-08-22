#pragma once

#include "basic.cpp"
#include "math.cpp"

constexpr uint maxalign = alignof(max_align_t);
constexpr static uint align(uint s) {return std::min(floor2(s), maxalign);}

typedef uint idx;

struct Memory {
    static constexpr idx MINLEN = 8;
    char* pointer;
    idx size;
    idx length;
    inline Memory() : Memory(0) {}
    inline Memory(idx l) {
        length = std::max(MINLEN, l);
        pointer = (char*)malloc(length);
        size = 0;
    }
    inline ~Memory() {
        free(pointer);
    }
    constexpr void release(idx index) {}
    inline idx alloc(uint s, uint a) {
        size = ((size - 1) | (a - 1)) + 1;
        idx ans = size;
        size += s;
        if (size > length) {
            length = ceil2(size);
            pointer = (char*)realloc(pointer, length);
        }
        return ans;
    }
    inline idx clloc(uint s, uint a) {
        idx ans = alloc(s, a);
        memset(ptr(ans), 0, s);
        return ans;
    }
    inline idx clone(idx p, uint s, uint a) { 
        idx ans = alloc(s, a);
        memcpy(ptr(ans), ptr(p), s);
        return ans;
    }
    inline idx ralloc(idx p, uint s, uint a) {
        idx ans = clone(p, s, a);
        release(p);
        return ans;
    }
    constexpr char* ptr(idx i) const {return pointer + i;}
    constexpr idx index(char* p) const {return (idx)(p - pointer);}
};

constexpr idx Memory::MINLEN;

Memory M = Memory();
