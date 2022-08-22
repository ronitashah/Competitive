#pragma once

#include "basic.cpp"

constexpr int MININT = 1 << 31;
constexpr int MAXINT = (uint)MININT - 1;
constexpr uint MAXUINT = ~(uint)0;
constexpr long MINLONG = (ulong)1 << 63;
constexpr long MAXLONG = (ulong)MINLONG - 1;
constexpr ulong MAXULONG = ~(ulong)0;

inline uint randuint() {return ((uint)rand() << 20) ^ ((uint)rand() << 10) ^ (uint)rand();}
inline int randint() {return randuint() >> 1;}
inline ulong randulong() {return ((ulong)randuint() << 32) ^ (ulong)randuint();}
inline long randlong() {return randulong() >> 1;}

inline uint rand(uint max) {return randuint() % max;}
inline int rand(int min, int max) {return randuint() % ((uint)(max - min)) + (uint)min;}
inline uint rand(uint min, uint max) {return randuint() % (max - min) + min;}
inline ulong rand(ulong max) {return randulong() % max;}
inline long rand(long min, long max) {return randulong() % ((ulong)(max - min)) + (ulong)min;}
inline ulong rand(ulong min, ulong max) {return randulong() % (max - min) + min;}

#define bits(E) (sizeof(E) << 3)

constexpr uint log2(uint num) {return 31 - __builtin_clz(num);}
constexpr uint log2(ulong num) {return 63 - __builtin_clz(num);}
constexpr uint floor2(uint num) {return 1 << log2(num);}
constexpr ulong floor2(ulong num) {return 1L << log2(num);}
constexpr uint ceil2(uint num) {return 1 << (log2((num) - 1) + 1);}
constexpr ulong ceil2(ulong num) {return 1L << (log2((num) - 1) + 1);}

inline int mod(int a, int b) {return (a >= 0) ? (a % b) : (b - 1 - ((-a - 1) % b));}
inline long mod(long a, long b) {return (a >= 0) ? (a % b) : (b - 1 - ((-a - 1) % b));}
ulong inv(ulong a, ulong b) {
	a %= b;
	if (a == 1) {
		return 1;
	}
	return (1 + b * (a - inv(b, a))) / a;
}
inline uint inv(uint a) {return (uint)inv(a, ((ulong)1) << 32);}

#define MOD(E) \
constexpr E cmpmod(E x) {return x;}

UINTS(MOD)
#undef MOD

#define MOD(E) \
constexpr E cmpmod(E x) {return x ^ (((E)1) << (bits(E) - 1));}

SINTS(MOD)
#undef MOD

constexpr uint cmpmod(float x) {
	uint i = floatint(x);
	return i ^ (((1 << (bits(uint) - 1)) - (i >> (bits(uint) - 1))) | (1 << (bits(uint) - 1)));
}
constexpr ulong cmpmod(double x) {
	ulong i = floatint(x);
	return i ^ ((((ulong)1 << (bits(ulong) - 1)) - (i >> (bits(ulong) - 1))) | ((ulong)1 << (bits(ulong) - 1)));
}
