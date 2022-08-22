#pragma once

#include <iostream>
#include <algorithm>
#include <time.h>
#include <string.h>
#include <bitset>
#include <chrono>

#define COMMA ,

#define same(A, B) (std::is_same<A, B>::value)

#define null nullptr

#define byte int8_t
#define ubyte uint8_t
#define short int16_t
#define ushort uint16_t
#define int int32_t
#define uint uint32_t
#define long int64_t
#define ulong uint64_t

constexpr uint floatint(float x) {return *(uint*)&x;}
constexpr ulong floatint(double x) {return *(ulong*)&x;}

constexpr float intfloat(uint x) {return *(float*)&x;}
constexpr double intfloat(ulong x) {return *(double*)&x;}

#define print(x) (std::cout << (x))
#define println(x) (std::cout << (x) << std::endl)

#define timestart(x) auto dfsadsfsd ## x = std::chrono::high_resolution_clock::now();
#define timeend(x) std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - dfsadsfsd ## x).count() / 1000000.

#define SINTS(F) F(byte) F(short) F(int) F(long)
#define UINTS(F) F(ubyte) F(ushort) F(uint) F(ulong)
#define INTS(F) SINTS(F) UINTS(F)
#define FLOATS(F) F(float) F(double)
#define NUMS(F) INTS(F) FLOATS(F)
#define MISCPRIMS(F) F(char) F(bool)
#define PRIMS(F) NUMS(F) MISCPRIMS(F)

#define signed(E) (same(E, byte) || same(E, short) || same(E, int) || same(E, long))

#define swapint(a, b) a^=b; b^=a; a^=b
#define swap(a, b, E) {E rewfrerg = a; a = b; b = rewfrerg;}

#define PRINTBITS(E) \
inline void printbits(E x) {print(std::bitset<sizeof(E) * 8>(x));} \
inline void printbitsln(E x) {println(std::bitset<sizeof(E) * 8>(x));}

INTS(PRINTBITS)
#undef PRINTBITS

#define CMPOPS(E) \
inline bool operator<(E x) const {return this->compare(x) < 0;} \
inline bool operator>(E x) const {return this->compare(x) > 0;} \
inline bool operator<=(E x) const {return this->compare(x) <= 0;} \
inline bool operator>=(E x) const {return this->compare(x) >= 0;} \
inline bool operator==(E x) const {return this->compare(x) == 0;} \
inline bool operator!=(E x) const {return this->compare(x) != 0;}

#define CMPF(E) \
inline int compare(E a, E b) {return a.compare(b);}

#define HASHF(E) \
inline ulong hash(E x) {return x.hash();}

#define RELF(E) \
inline void release(E x) {x.release();}

#define METHF(E, T) \
namespace my { \
T CMPF(E) T HASHF(E) T RELF(E) \
}

#define CMPD(E) \
inline int compare(E a, E b);

#define HASHD(E) \
inline ulong hash(E x);

#define RELD(E) \
inline void release(E x);

#define PRINTD(E) \
std::ostream& operator<<(std::ostream& os, E x);

#define METHD(E, T) \
namespace my { \
T CMPD(E) T HASHD(E) T RELD(E) \
} \
T PRINTD(E)

namespace my {

#define CMP(E) \
constexpr int compare(E a, E b) {return a < b ? -1 : b < a ? 1 : 0;}
PRIMS(CMP)
#undef CMP

#define REL(E) \
constexpr void release(E x) {}
PRIMS(REL)
#undef REL

#define HASH(E) \
constexpr ulong hash(E x) {return x;}
INTS(HASH);
MISCPRIMS(HASH)
#undef HASH

#define HASH(E) \
constexpr ulong hash(E x) {return floatint(x);}
FLOATS(HASH)
#undef HASH

}

template<class A, class B>
struct T2;
namespace my {
template<class A, class B>
CMPD(T2<A COMMA B>)
template<class A, class B>
RELD(T2<A COMMA B>)
template<class A, class B>
HASHD(T2<A COMMA B>)
}
template<class A, class B>
PRINTD(T2<A COMMA B>)

struct Roll;
namespace my {
RELD(Roll)
HASHD(Roll)
}
PRINTD(Roll)

template<class E>
struct Array;
METHD(Array<E>, template<class E>)

template<class E>
struct List;
METHD(List<E>, template<class E>)

template<class E>
struct Heap;
namespace my {
template<class E>
RELD(Heap<E>)
}
template<class E>
PRINTD(Heap<E>);

template<class A, class B>
struct T2 {
    A a;
    B b;
    constexpr T2() {}
    constexpr T2(A a, B b) {
        this->a = a;
        this->b = b;
    } 
    inline void release() {
        my::release(a);
        my::release(b);      
    }
    inline ulong hash() const;
    inline int compare(T2<A, B> t) const {
        int ans = my::compare(a, t.a);
        return ans != 0 ? ans : my::compare(b, t.b);
    }
    CMPOPS(T2<A COMMA B>)
};

template<class A, class B>
std::ostream& operator<<(std::ostream& os, T2<A, B> t) {
    os << "(" << t.a << ", " << t.b << ")";
    return os;
}

namespace my {
template<class A, class B>
CMPF(T2<A COMMA B>)
template<class A, class B>
RELF(T2<A COMMA B>)
}