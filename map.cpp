#pragma once

#include "list.cpp"

template<class K, class V>
struct Map {
    constexpr static uint AVBYTES = 16;
    Array<List<T2<K, V>>> arr;
    uint size;
    inline Map() {
        size = 0;
        arr = Array<List<T2<K, V>>>(1);
        arr.fill(List<T2<K, V>>());
    }
    static uint find(List<T2<K, V>> l, K k) {
        T2<K, V>* p = l.ptr();
        for (uint x = 0; x < l.size; x++) {
            if ((p + x)->a == k) {
                return x;
            }
        }
        return MAXUINT;
    }
    inline void release() const {arr.release();}
    constexpr uint index(K k) const {return k & (arr.length - 1);}
    idx id(K k) const {
        List<T2<K, V>> l = arr[index(k)];
        uint i = find(l, k);
        if (i == MAXUINT) {
            return MAXUINT;
        }
        return M.index((char*)&(l.ptr(i)->b));
    }
    inline bool contains(K k) const {return id(k) != MAXUINT;}
    inline V get(K k) const {
        idx i = id(k);
        if (i == MAXUINT) {
            return 0;
        }
        return *(V*)M.ptr(i);
    }
    inline void force(K k, V v) {
        uint i = index(k);
        List<T2<K, V>> l = arr[i];
        l.add(T2<K, V>(k, v));
        arr.set(i, l);
        size++;
    }
    inline void force(T2<K, V> t) {force(t.a, t.b);}
    void resize(uint l) {
        Array<List<T2<K, V>>> next = Array<List<T2<K, V>>>(l);
        next.fill(List<T2<K, V>>());
        swap(arr, next, Array<List<T2<K COMMA V>>>);
        size = 0;
        for (uint x = 0; x < next.length; x++) {
            List<T2<K, V>> l = next[x];
            for (uint y = 0; y < l.size; y++) {
                force(l[y]);
            }
        }
    }
    inline bool add(K k, V v) {
        uint i = index(k);
        List<T2<K, V>> l = arr[i];
        uint f = find(l, k);
        if (f != MAXUINT) {
            l.set(f, T2<K, V>(k, v));
            return false;
        }
        l.add(T2<K, V>(k, v));
        arr.set(i, l);
        size++;
        if (size * sizeof(K) == arr.length * AVBYTES) {
            resize(arr.length * 2);
        }
        return true;
    }
    inline bool add(T2<K, V> t) {return add(t.a, t.b);}
    inline bool remove(K k) {
        uint i = index(k);
        List<T2<K, V>> l = arr[i];
        uint f = find(l, k);
        if (f == MAXUINT) {
            return false;
        }
        l.set(f, l.head());
        l.remove();
        arr.set(i, l);
        size--;
        if (SMALLSIZE && size * sizeof(K) < arr.length * AVBYTES / 2) {
            resize(std::max((uint)1, arr.length / 2));
        }
        return true;
    }
};

template<class K, class V>
constexpr uint Map<K, V>::AVBYTES;

template<class K, class V>
inline std::ostream& operator<<(std::ostream& os, Map<K, V> m) {
    return os << m.arr;
}

namespace my {
template<class K, class V>
RELF(Map<K COMMA V>)
}
