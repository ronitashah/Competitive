#include "set.cpp"
#include "heap.cpp"
#include "map.cpp"

int main() {
    Array<ulong> a = Array<ulong>(1 << 2);
    Array<ulong> b = Array<ulong>(a.length);
    Array<ulong> c = Array<ulong>(a.length);
    for (uint x = 0; x < a.length; x++) {
        a.set(x, randulong());
        b.set(x, randulong());
        c.set(x, randulong());
    }
    Map<ulong, ulong> m = Map<ulong, ulong>();
    for (uint x = 0; x < a.length; x++) {
        if (!m.add(a[x], b[x])) {
            println("bad");
        }
    }
    for (uint x = 0; x < a.length; x++) {
        uint i = m.id(a[x]);
        if (i == MAXUINT || *(ulong*)M.ptr(i) != b[x]) {
            println("bad2");
        }
        *(ulong*)M.ptr(i) = c[x];
    }
    for (uint x = 0; x < a.length; x++) {
        if (m.get(a[x]) != c[x]) {
            println("bad3");
        }
    }
    println(m);
    println("completed");
    return 0;
}