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
        index = 0;
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
    static Array<uint> indexes(uint length) {
        Array<uint> ans = Array<uint>(length);
        for (uint x = 0; x < length; x++) {
            ans[x] = x;
        }
        return ans;
    }
    inline void fill() const {memset(ptr(), 0, length * sizeof(E));}
    void fill(E v) const {
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
    constexpr E* begin() const {return ptr();}
    constexpr E* end() const {return ptr(length);}
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

    uint higher(E v) {
        BINSEARCH(v < get PAREN0, PAREN1, length);
    }
    uint ceil(E v) {
        BINSEARCH(v <= get PAREN0, PAREN1, length);
    }
    uint inline lower(E v) {
        return ceil(v) - 1;
    }
    uint inline floor(E v) {
        return higher(v) - 1;
    }
    int inline binsearch(E v) {
        uint ans = ceil(v);
        if (ans == length || v < get(ans)) {
            return -ans - 1;
        }
        return ans;
    }
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
    template <class V>
    void sort(Array<V> move) const;
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
        counts.fill(); \
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
    if (ptr() != p0) { \
        memcpy(p1, p0, length * sizeof(E)); \
    } \
    space.release(); \
    counts.release(); \
} \
template<> \
template<class V> \
void Array<E>::sort(Array<V> move) const { \
    Array<E> space = Array<E>(length); \
    Array<V> movespace = Array<V>(length); \
    Array<uint> counts = Array<uint>(1 << RADIX); \
    E* p0 = ptr(); \
    E* p1 = space.ptr(); \
    uint* p2 = counts.ptr(); \
    V* m0 = move.ptr(); \
    V* m1 = movespace.ptr(); \
    for (ubyte r = 0; r < bits(E); r += RADIX) { \
        counts.fill(); \
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
            uint i = (*(p2 + ((cmpmod(*(p0 + x)) >> r) & ((1 << RADIX) - 1))))++; \
            *(p1 + i) = *(p0 + x); \
            *(m1 + i) = *(m0 + x); \
        } \
        swap(p0, p1, E*); \
        swap(m0, m1, V*); \
    } \
    if (ptr() != p0) { \
        memcpy(p1, p0, length * sizeof(E)); \
        memcpy(m1, m0, length * sizeof(V)); \
    } \
    space.release(); \
    counts.release(); \
    movespace.release(); \
}

NUMS(RADIXSORT)
#undef RADIXSORT

METHF(Array<E>, template<class E>)
