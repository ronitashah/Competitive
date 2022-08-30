#include "math.cpp"

struct Comm {
    static ulong rmult;
    static ulong radd;
    static ulong rxor;
    ulong val = 0;
    constexpr Comm() {}
    constexpr Comm(ulong v) {val = v;}
    template<class E>
    Comm(const E* p, uint s) {
        for (uint x = 0; x < s; x++) {
            add(my::hash(p[x]));
        }
    }
    template<class E>
    inline Comm(std::initializer_list<E> l) : Comm(l.begin(), l.size()) {}
    constexpr Comm clone() const {return Comm(val);}
    constexpr void release() const {}
    constexpr ulong hash() const {return val;}
    inline ulong convert(ulong v) {return (v * rmult + radd) ^ rxor;}
    inline void add(ulong v) {val += convert(v);}
    inline void add(ulong v, ulong n) {val += convert(v) * n;}
    constexpr void add(Comm c) {val += c.val;}
    constexpr void add(Comm c, ulong n) {val += c.val * n;}
    inline void operator+=(ulong v) {add(v);}
    constexpr void operator+=(Comm c) {add(c);}
    inline Comm operator+(ulong v) const {
        Comm ans = clone();
        ans += v;
        return ans;
    }
    constexpr Comm operator+(Comm c) const {
        Comm ans = clone();
        ans += c;
        return ans;
    }
    inline void sub(ulong v) {val -= convert(v);}
    inline void sub(ulong v, ulong n) {val -= convert(v) * n;}
    constexpr void sub(Comm c) {val -= c.val;}
    constexpr void sub(Comm c, ulong n) {val -= c.val * n;}
    inline void operator-=(ulong v) {sub(v);}
    constexpr void operator-=(Comm c) {sub(c);}
    inline Comm operator-(ulong v) const {
        Comm ans = clone();
        ans -= v;
        return ans;
    }
    constexpr Comm operator-(Comm c) const {
        Comm ans = clone();
        ans -= c;
        return ans;
    }
    constexpr int compare(Comm c) const {
        return my::compare(hash(), c.hash());
    }
    CMPOPS(Comm)
};

ulong Comm::rmult = randulong() | 1;
ulong Comm::radd = randulong();
ulong Comm::rxor = randulong();

inline std::ostream& operator<<(std::ostream& os, Comm c) {
    os << c.val;
    return os;
}

namespace my {
RELF(Comm)
HASHF(Comm)
}
