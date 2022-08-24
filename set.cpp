#include "list.cpp"

template<class E>
struct Set {
    constexpr static uint AVBYTES = 16;
    uint size;
    Array<List<E>> arr;
    constexpr Set() : size(0) {
        arr = Array<List<E>>(1);
        arr.fill(List<E>());
    }
    constexpr uint index(E v) const {return v & (arr.length - 1);}
    bool contains(E v) const {
        List<E> l = arr[index(v)];
        E* p = l.ptr();
        for (uint x = 0; x < l.size; x++) {
            if (*(p + x) == v) {
                return true;
            }
        }
        return false;
    }
    void resize(uint l) {
        Array<List<E>> next = Array<List<E>>(l);
        next.fill(List<E>());
        swap(arr, next, Array<List<E>>);
        size = 0;
        for (uint x = 0; x < next.length; x++) {
            List<E> l = next[x];
            for (uint y = 0; y < l.size; y++) {
                force(l[y]);
            }
        }
    }
    inline void force(E v) {
        uint i = index(v);
        List<E> l = arr[i];
        l.add(v);
        arr[i] = l;
        size++;
    }
    inline void add(E v) {
        if (size * sizeof(E) > arr.length * AVBYTES) {
            resize(arr.length * 2);
        }
        force(v);
    }
    bool remove(E v) {
        uint i = index(v);
        List<E> l = arr[i];
        E* p = l.ptr();
        for (uint x = 0; x < l.size; x++) {
            if (*(p + x) == v) {
                *(p + x) = *(p + l.size - 1);
                l.remove();
                arr[i] = l;
                size--;
                if (SMALLSIZE && size * sizeof(E) < arr.length * AVBYTES) {
                    resize(std::max((uint)1, arr.length / 2));
                }
                return true;
            }
        }
        return false;
    }
};

template<class E>
constexpr uint Set<E>::AVBYTES;
