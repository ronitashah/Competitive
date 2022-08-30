#pragma once

#include "list.cpp"

template<class K, class V>
struct Map {
    constexpr static uint AVBYTES = 16;
    Array<List<K>> keys;
    Array<List<V>> vals;
    uint size;
    inline Map() {
        size = 0;
        keys = Array<List<K>>(1);
        keys.init();
        vals = Array<List<V>>(1);
        vals.init();
    }
    Map(Array<K> ks, Array<V> vs) : Map() {
        for (uint x = 0; x < ks.length; x++) {
            add(ks[x], vs[x]);
        }
    } 
    inline void release() const {
        keys.release();
        vals.release();
    }
    constexpr uint index(K k) const {return k & (keys.length - 1);}
    inline idx id(K k) const {
        uint i = index(k);
        uint f = keys[i].find(k);
        if (f == MAXUINT) {
            return nullidx;
        }
        return vals[i].id(f);
    }
    inline bool contains(K k) const {return id(k) != nullidx;}
    inline V get(K k) const {
        idx i = id(k);
        if (i == nullidx) {
            return 0;
        }
        return *(V*)M.ptr(i);
    }
    inline V operator[](K k) const {return get(k);}
    inline void force(K k, V v) {
        uint i = index(k);
        List<K> lk = keys[i];
        lk.add(k);
        keys.ptr()[i] = lk;
        List<V> lv = vals[i];
        lv.add(v);
        vals.ptr()[i] = lv;
        size++;
    }
    void resize(uint l) {
        Array<List<K>> nextk = Array<List<K>>(l);
        nextk.init();
        Array<List<V>> nextv = Array<List<V>>(l);
        nextv.init();
        swap(keys, nextk, Array<List<K>>);
        swap(vals, nextv, Array<List<V>>);
        size = 0;
        for (uint x = 0; x < nextk.length; x++) {
            for (uint y = 0; y < nextk[x].size; y++) {
                force(nextk[x][y], nextv[x][y]);
            }
        }
    }
    inline bool add(K k, V v) {
        uint i = index(k);
        List<K> lk = keys[i];
        uint f = lk.find(k);
        if (f != MAXUINT) {
            lk.ptr()[f] = k;
            vals[i].ptr()[f] = v;
            return false;
        }
        lk.add(k);
        keys.ptr()[i] = lk;
        List<V> lv = vals[i];
        lv.add(v);
        vals.ptr()[i] = lv;
        size++;
        if (size * sizeof(K) == keys.length * AVBYTES) {
            resize(keys.length * 2);
        }
        return true;
    }
    inline bool remove(K k) {
        uint i = index(k);
        List<K> lk = keys[i];
        uint f = lk.find(k);
        if (f == MAXUINT) {
            return false;
        }
        lk.ptr()[f] = lk.head();
        lk.remove();
        keys.ptr()[i] = lk;
        List<V> lv = vals[i];
        lv.ptr()[f] = lv.head();
        lv.remove();
        vals.ptr()[i] = lv;
        size--;
        if (SMALLSIZE && size * sizeof(K) < keys.length * AVBYTES / 2) {
            resize(std::max((uint)1, keys.length / 2));
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
