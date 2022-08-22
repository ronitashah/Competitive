#include "array.cpp"

template<class E>
struct Heap {
    Array<E> arr;
    uint size;
    constexpr Heap() {
        arr = Array<E>(1);
        size = 0;
    }
    constexpr E* ptr() {return arr.ptr();}
    void add(E val) {
        if (size == arr.length) {
            arr.resize(arr.length * 2 + 1);
        }
        E* p = ptr();
        uint cur = size;
        for (;cur > 0;) {
            uint parent = (cur - 1) >> 1;
            if (*(p + parent) < val) {
                *(p + cur) = *(p + parent);
                cur = parent;
                continue;
            }
            break;
        }
        *(p + cur) = val;
        size++;
    }
};

template <class E>
std::ostream& operator<<(std::ostream& os, Heap<E> h) {
    E* p = h.ptr();
    for (uint x = 0; x < h.size; x++) {
        if (ceil2(x + 1) == x + 1) {
            os << '\n';
        }
        os << *(p + x) << " ";
    }
    return os;
}
