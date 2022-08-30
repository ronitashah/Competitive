#pragma once

#include "array.cpp"

template<class E>
struct Fen {
    Array<E> arr;
    constexpr Fen() {arr = Array<E>();}
    inline Fen(uint l) {
        arr = Array<E>(2 * ceil2(l) - 1);
        arr.init();
    }
    Fen(Array<E> a) : Fen(a.length) {
        for (uint x = 0; x < a.length; x++) {
            arr.set(x + arr.length / 2, a[x]);
        }
        for (uint x = arr.length / 2 - 1; x != MAXUINT; x--) {
            update(x);
        }
    }
    constexpr uint size() const {return (arr.length + 1) / 2 + 1;}
    constexpr E operator[](uint i) const {return arr[arr.length / 2 + i];}
    constexpr void update(uint i) const {arr.set(i, arr[2 * i + 1] + arr[2 * i + 2]);}
    void set(uint i, E v) {
        i += arr.length / 2;
        arr.set(i, v);
        i = (i - 1) / 2;
        for (;i != 0; i = (i - 1) / 2) {
            update(i);
        }
        update(0);
    }
    E sum(uint from, uint to) {
        uint x0 = arr.length / 2 + from;
        uint x1 = arr.length / 2 + to - 1;
        E v0 = E();
        E v1 = E();
        for (;x0 < x1; x0 /= 2, x1 = x1 / 2 - 1) {
            if ((x0 & 1) == 0) {
                v0 += arr[x0];
            }
            if (x1 & 1) {
                v1 = arr[x1] + v1;
            }
        }
        if (x0 != x1) {
            return v0 + v1;
        }
        return v0 + arr[x0] + v1;
    }
    inline E sum(uint to) {return sum(0, to);}
    inline E sum() {return sum((arr.length + 1) / 2);}
    uint higher(E target) {
        if (arr.length == 0 || !(target < arr[0])) {
            return (arr.length + 1) / 2 + 1;
        }
        E sum = E();
        uint cur = 1;
        for (;cur < arr.length;) {
            E next = sum + arr[cur];
            if (target < next) {
                cur = 2 * cur + 1;
                continue;
            }
            sum = next;
            cur = 2 * cur + 3;
        }
        uint ans = cur / 2 - arr.length / 2 + 1;
        if (ans == 1 && target < E()) {
            return 0;
        }
        return ans;
    }
    uint ceil(E target) {
        if (arr.length == 0 || arr[0] < target) {
            return (arr.length + 1) / 2 + 1;
        }
        E sum = E();
        uint cur = 1;
        for (;cur < arr.length;) {
            E next = sum + arr[cur];
            if (!(next < target)) {
                cur = 2 * cur + 1;
                continue;
            }
            sum = next;
            cur = 2 * cur + 3;
        }
        uint ans = cur / 2 - arr.length / 2 + 1;
        if (ans == 1 && !(E() < target)) {
            return 0;
        }
        return ans;
    }
    uint inline lower(E v) {
        return ceil(v) - 1;
    }
    uint inline floor(E v) {
        return higher(v) - 1;
    }
    int inline binsearch(E v) {
        uint ans = ceil(v);
        if (ans == size() || v < sum(ans)) {
            return -ans - 1;
        }
        return ans;
    }
};

template <class E>
std::ostream& operator<<(std::ostream& os, Fen<E> f) {
    E* p = f.arr.ptr();
    for (uint x = 0; x < f.arr.length; x++) {
        if (ceil2(x + 1) == x + 1) {
            os << '\n';
        }
        os << p[x] << " ";
    }
    return os;
}