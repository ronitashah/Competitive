#pragma once

#include "list.cpp"

template<class E>
struct Set {
    constexpr static uint AVBYTES = 16;
    Array<List<E>> arr;
    uint size = 0;
    constexpr Set() {
        arr = Array<List<E>>(1);
        arr.init();
    }
    Set(Array<E> a) : Set() {
        for (uint x = 0; x < a.length; x++) {
            add(a[x]);
        }
    }
    inline void release() const {arr.release();}
    constexpr uint index(E v) const {return v & (arr.length - 1);}
    inline bool contains(E v) const {return arr[index(v)].contains(v);}
    inline bool operator[](E v) const {return contains(v);}
    inline void force(E v) {
        uint i = index(v);
        List<E> l = arr[i];
        l.add(v);
        arr.ptr()[i] = l;
        size++;
    }
    void resize(uint l) {
        Array<List<E>> next = Array<List<E>>(l);
        next.init();
        swap(arr, next, Array<List<E>>);
        size = 0;
        for (uint x = 0; x < next.length; x++) {
            List<E> l = next[x];
            for (uint y = 0; y < l.size; y++) {
                force(l[y]);
            }
        }
    }
    inline bool add(E v) {
        uint i = index(v);
        if (arr[i].contains(v)) {
            return false;
        }
        List<E> l = arr[i];
        l.add(v);
        arr.ptr()[i] = l;
        size++;
        if (size * sizeof(E) > arr.length * AVBYTES) {
            resize(arr.length * 2);
        }
        return true;
    }
    inline bool remove(E v) {
        uint i = index(v);
        List<E> l = arr[i];
        uint f = l.find(v);
        if (f == MAXUINT) {
            return false;
        }
        l.ptr()[f] = l.head();
        l.remove();
        arr.ptr()[i] = l;
        size--;
        if (SMALLSIZE && size * sizeof(E) < arr.length * AVBYTES / 2) {
            resize(std::max((uint)1, arr.length / 2));
        }
        return true;
    }
};

template<class E>
constexpr uint Set<E>::AVBYTES;

namespace my {
template<class E>
RELF(Set<E>)
}
template<class E>
inline std::ostream& operator<<(std::ostream& os, Set<E> s) {
    return os << s.arr;
}
