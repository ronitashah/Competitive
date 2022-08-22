#pragma once

#include "memory.cpp"
#include "roll.cpp"

template<class E>
struct Array {
    static constexpr uint INSERTION = 32;
    static constexpr ubyte RADIX = 8;
    idx index;
    uint length;
    constexpr Array() {
        index = MAXUINT;
        length = 0;
    }
    inline Array(uint l) {
        index = M.alloc(l * sizeof(E), alignof(E));
        length = l;
    }
    inline Array(const E* p, uint l) : Array(l) {
        copy(p);
    }
    inline Array(std::initializer_list<E> l) : Array(l.begin(), l.size()) {}
    constexpr Array(idx i, uint l) : index(i), length(l) {}
    inline void init() const {memset(ptr(), 0, length * sizeof(E));}
    void init(E v) const {
        E* p = ptr();
        for (uint x = 0; x < length; x++) {
            *(p + x) = v;
        }
    }
    inline void copy(const E* p) const {memcpy(ptr(), p, length * sizeof(E));}
    inline void copy(Array<E> a) const {copy(a.ptr());}
    inline void release() const {
        for (uint x = 0; x < length; x++) {
            my::release(get(x));
        }
        M.release(index);
    }
    inline ulong hash() const {return Roll::hash<E>(ptr(), length);}
    inline Array<E> clone() const {return clone(length);}
    inline Array<E> clone(uint l) const {return Array<E>(M.clone(index, l * sizeof(E), alignof(E)), l);}
    inline void resize(uint l) {
        index = M.ralloc(index, l * sizeof(E), alignof(E));
        length = l;
    }
    constexpr E* ptr() const {return (E*)M.ptr(index);}
    constexpr E* ptr(uint i) const {return ptr() + i;}
    constexpr E& operator[] (uint i) const {return *ptr(i);}
    constexpr E get(uint i) const {return *ptr(i);}
    constexpr E head() const {return get(length - 1);}
    constexpr void set(uint i, E v) const {*ptr(i) = v;}
    constexpr Array<E> view(uint start, uint end) const {return Array<E>(index + start * sizeof(E), end - start);} // !!!
    constexpr Array<E> view(uint l) const {return view(0, l);} // !!!
    int compare(Array<E> a) const {
        uint min = std::min(length, a.length);
        for (uint x = 0; x < min; x++) {
            int c = my::compare(get(x), a[x]);
            if (c != 0) {
                return c;
            }
        }
        return length - a.length;
    }
    
    CMPOPS(Array<E>)

    void sort() const {
        Array<uint> stack = Array<uint>(4);
        uint a = 0;
        stack[0] = length;
        uint size = 1;
        for (;size >= 1;) {
            uint b = stack[size - 1];
            E* p = ptr();
            if (b - a < INSERTION) {
                for (uint i = a + 1; i < b; i++) {
                    E v = *(p + i);
                    for (uint x = i;;x--) {
                        if (x == a || *(p + x - 1) < v) {
                            *(p + x) = v;
                            break;
                        }
                        *(p + x) = *(p + x - 1);
                    }
                }
                a = b + 1;
                size--;
                continue;
            }
            E v1 = *(p + a);
            E v2 = *(p + (a + b) / 2);
            E v3 = *(p + b - 1);
            uint i = v1 < v2 ? v2 < v3 ? (a + b) / 2 : v1 < v3 ? b - 1 : a : v1 < v3 ? a : v2 < v3 ? b - 1 : (a + b) / 2;
            E v = *(p + i);
            *(p + i) = *(p + a);
            *(p + a) = v;
            i = a;
            for (uint x = a + 1; x < b; x++) {
                E q = *(p + x);
                int c = my::compare(v, q);
                if (c < 0 || (c == 0 && (x & 1))) {
                    continue;
                }
                *(p + i) = q;
                i++;
                *(p + x) = *(p + i);
                *(p + i) = v;
            }
            if (size == stack.length) {
                stack.resize(stack.length << 1);
            }
            stack[size] = i;
            size++;
        }
        stack.release();
    }
};

template <class E>
std::ostream& operator<<(std::ostream& os, Array<E> a) {
    if (a.length == 0) {
        os << "[]";
        return os;
    }
    os << "[" << a[0];
    for (uint x = 1; x < a.length; x++) {
        os << ", " << a[x];
    }
    os << "]";
    return os;
}

#define RADIXSORT(E) \
template<> \
void Array<E>::sort() const { \
    Array<E> space = Array<E>(length); \
    Array<uint> counts = Array<uint>(1 << RADIX); \
    E* p0 = ptr(); \
    E* p1 = space.ptr(); \
    uint* p2 = counts.ptr(); \
    for (ubyte r = 0; r < bits(E); r += RADIX) { \
        counts.init(); \
        for (uint x = 0; x < length; x++) { \
            (*(p2 + ((cmpmod(*(p0 + x)) >> r) & ((1 << RADIX) - 1))))++; \
        } \
        uint sum = 0; \
        for (uint x = 0; x < (1 << RADIX); x++) { \
            uint v = *(p2 + x); \
            *(p2 + x) = sum; \
            sum += v; \
        } \
        for (uint x = 0; x < length; x++) { \
            *(p1 + (*(p2 + ((cmpmod(*(p0 + x)) >> r) & ((1 << RADIX) - 1))))++) = *(p0 + x); \
        } \
        swap(p0, p1, E*); \
    } \
    if (bits(E) == RADIX) { \
        memcpy(p1, p0, length * sizeof(E)); \
    } \
    space.release(); \
    counts.release(); \
}

NUMS(RADIXSORT)
#undef RADIXSORT

METHF(Array<E>, template<class E>)
