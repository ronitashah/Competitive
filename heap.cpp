#include "array.cpp"

template<class E>
struct Heap {
    Array<E> arr;
    uint size;
    constexpr Heap() {
        arr = Array<E>();
        size = 0;
    }
    inline Heap(Array<E> a) {
        arr = a.clone(ceil2(a.length + 1) - 1);
        size = a.length;
        fix();
    }
    inline Heap(const E* p, uint s) : Heap(Array<E>(p, s)) {}
    inline Heap(std::initializer_list<E> l) : Heap(l.begin(), l.size()) {}
    constexpr void release() const {arr.release();}
    constexpr E* ptr() const {return arr.ptr();}
    constexpr E peek() const {return arr[0];}
    constexpr uint fix(uint i) const {
        E* p = ptr();
        uint min = 2 * i + 1 >= size || *(p + i) < *(p + 2 * i + 1) ? 2 * i + 2 >= size || *(p + i) < *(p + 2 * i + 2) ? i : 2 * i + 2 : 2 * i + 2 >= size || *(p + 2 * i + 1) < *(p + 2 * i + 2) ? 2 * i + 1 : 2 * i + 2;
        swap(*(p + i), *(p + min), E);
        return min;
    }
    void fix() const {
        if (size < 2) {
            return;
        }
        for (uint x = (size - 2) / 2; x != MAXUINT; x--) {
            for (uint cur = x;;) {
                uint next = fix(cur);
                if (cur == next) {
                    break;
                }
                cur = next;
            }
        }
    }
    void push(E val) {
        if (size == arr.length) {
            arr.resize(arr.length * 2 + 1);
        }
        E* p = ptr();
        uint cur = size;
        for (;cur > 0;) {
            uint parent = (cur - 1) / 2;
            if (val < *(p + parent)) {
                *(p + cur) = *(p + parent);
                cur = parent;
                continue;
            }
            break;
        }
        *(p + cur) = val;
        size++;
    }
    E pop() {
        E ans = peek();
        size--;
        arr[0] = size;
        if (SMALLSIZE && size == arr.length / 4) {
            arr.resize(arr.length / 2);
        }
        for (uint cur = 0;;) {
            uint next = fix(cur);
            if (next == cur) {
                return ans;
            }
            cur = next;
        }
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
