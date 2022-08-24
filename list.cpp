#pragma once

#include "array.cpp"

template<class E>
struct List {
    Array<E> arr;
    uint size;
    inline List() {
        arr = Array<E>();
        size = 0;
    }
    inline List(uint s) {
        arr = Array<E>(s);
        size = s;
    }
    constexpr List(Array<E> a, uint s) {
        arr = a;
        size = s;
    }
    inline List(Array<E> a) : List(a, a.length) {}
    inline List(std::initializer_list<E> l) : List(Array<E>(l)) {}
    inline List(const E* p, uint l) : List(Array<E>(p, l), l) {}
    constexpr E* ptr() const {return arr.ptr();}
    constexpr E* ptr(uint i) const {return arr.ptr(i);}
    constexpr idx id(uint i) const {return arr.id(i);}
    constexpr E get(uint i) const {return arr.get(i);}
    constexpr E head() const {return view().head();}
    constexpr void set(uint i, E v) const {arr.set(i, v);}
    constexpr E operator[](uint i) const {return *ptr(i);}
    inline Array<E> array() const {return arr.clone(size);}
    constexpr Array<E> view(uint from, uint to) const {return arr.view(from, to);}
    constexpr Array<E> view(uint l) const {return arr.view(l);}
    constexpr Array<E> view() const {return arr.view(size);}
    constexpr E* begin() const {return ptr();}
    constexpr E* end() const {return view().end();}
    constexpr void release() const {view().release();}
    inline int compare(List<E> l) const {return view().compare(l.view());}
    CMPOPS(List<E>)
    inline ulong hash() const {return view().hash();}
    inline void sort() const {view().sort();}
    template<class V>
    inline void sort(Array<V> move) const {view().sort(move);}
    inline uint find(E v) const {return view().find(v);}
    inline bool contains(E v) const {return view().contains(v);}
    inline void add() {
        if (size == arr.length) {
            arr.resize(std::max((uint)1, 2 * arr.length));
        }
        size++;
    }
    inline void add(E v) {
        add();
        set(size - 1, v);
    }
    inline void add(uint i, E v) {
        add();
        E* p = ptr(i);
        memmove(p + 1, p, (size - i - 1) * sizeof(E));
        *p = v;
    }
    void add(Array<uint> indexes, Array<E> vals) {
        for (uint x = 0; x < indexes.length; x++) {
            add();
        }
        E* p = ptr();
        uint* i = indexes.ptr();
        E* v = vals.ptr();
        uint cur = indexes.length - 1;
        for (uint x = size - 1; x != MAXUINT; x--) {
            if (cur != MAXUINT && *(i + cur) == x - cur) {
                *(p + x) = *(v + cur);
                cur--;
                continue;
            }
            *(p + x) = *(p + x - cur - 1);
        }
    }
    inline E remove() {
        E ans = head();
        if (SMALLSIZE && size == (arr.length >> 2) && arr.length > 1) {
            arr.resize(arr.length >> 1);
        }
        size--;
        return ans;
    }
    inline E remove(uint i) {
        E ans = head();
        E* p = ptr(i);
        memmove(p, p + 1, (size - i - 1) * sizeof(E));
        remove();
        return ans;
    }
    void remove(Array<uint> indexes) {
        E* p = ptr();
        uint* i = indexes.ptr();
        uint cur = 0;
        for (uint x = 0; x < size; x++) {
            if (cur < indexes.length && *(i + cur) == x) {
                cur++;
                continue;
            }
            *(p + x - cur) = *(p + x);
        }
        for (uint x = 0; x < cur; x++) {
            remove();
        }
    }
};

template<class E>
inline std::ostream& operator<<(std::ostream& os, List<E> l) {
    return os << l.view();
}

METHF(List<E>, template<class E>)
