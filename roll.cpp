#pragma once

#include "math.cpp"

//val = (front) a + b * r + c * r^2+... (back)
//pow is the next power of r(not used yet)
//hash = val ^ pow + add

struct Roll {
    static ulong radix;
    static ulong randadd;
    ulong val;
    ulong pow;
    constexpr Roll() : Roll(0, 1) {}
    constexpr Roll(ulong v, ulong p) : val(v), pow(p) {}
    template<class A, class B>
    inline Roll(T2<A, B> t) {
        val = my::hash(t.a) * radix + my::hash(t.b);
        pow = radix * radix;
    }
    template<class E>
    Roll(const E* p, uint s) {
        val = 0;
        pow = 1;
        for (uint x = 0; x < s; x++) {
            val = val * radix + my::hash(*(p + x));
            pow *= radix;
        }
    }
    template<class E>
    inline Roll(std::initializer_list<E> l) : Roll(l.begin(), l.size()) {}
    constexpr void release() {}
    template<class A, class B>
    static inline ulong hash(T2<A, B> t) {return Roll(t).hash();}
    template<class E>
    static inline ulong hash(E* p, uint s) {return Roll(p, s).hash();}
    inline ulong hash() const {return (val ^ pow) + randadd;}
    constexpr Roll clone() const {return Roll(val, pow);}
    inline void roll(ulong v) {
        val = val * radix + v;
        pow *= radix;
    }
    inline void operator+=(ulong v) {roll(v);}
    inline Roll operator+(ulong v) {
        Roll ans = clone();
        ans += v;
        return ans;
    }
    constexpr void roll(Roll r) {
        val = val * r.pow + r.val;
        pow *= r.pow;
    }
    constexpr void operator+=(Roll r) {roll(r);}
    constexpr Roll operator+(Roll r) {
        Roll ans = clone();
        ans += r;
        return ans;
    }
    inline void rollback(ulong v) {
        val += pow * v;
        pow *= radix;
    }
    constexpr void rollback(Roll r) {
        val += pow * r.val;
        pow *= r.pow;
    }
    inline void rollunroll(ulong roll, ulong unroll) {val = val * radix + roll - unroll * pow;}
    constexpr void rollunroll(Roll roll, Roll unroll) {val = val * roll.pow + roll.val - unroll.val * pow;}
};

namespace my {
RELF(Roll)
HASHF(Roll)
}

inline std::ostream& operator<<(std::ostream& os, Roll r) {
    os << "(" << r.val << " " << r.pow << ")";
    return os;
}

ulong Roll::radix = randulong() | 1;
ulong Roll::randadd = randulong();

template<class A, class B>
inline ulong T2<A, B>::hash() const {return Roll::hash(*this);}

namespace my {
template<class A, class B>
HASHF(T2<A COMMA B>)
}
