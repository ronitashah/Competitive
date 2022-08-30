#include "set.cpp"
#include "heap.cpp"
#include "map.cpp"
#include "fen.cpp"
#include "comm.cpp"
#include "dst.cpp"

int main() {
    Array<uint> a = Array<uint>(1 << 12);
    println(a.length);
    for (uint x = 0; x < a.length; x++ ) {
        a.ptr()[x] = randuint();
    }
    Array<Array<Comm>> b = Array<Array<Comm>>(a.length);
    for (uint from = 0; from < a.length; from++) {
        b.ptr()[from] = Array<Comm>(a.length + 1);
        b[from].ptr()[from] = Comm();
        for (uint to = from + 1; to <= a.length; to++) {
            b[from].ptr()[to] = b[from][to - 1] + a[to - 1];
        }
    }
    Array<Comm> c = Array<Comm>(a.length + 1);
    *c.ptr() = Comm();
    for (uint x = 0; x < a.length; x++) {
        c.ptr()[x + 1] = c[x] + a[x];
    }
    for (uint from = 0; from < a.length; from++) {
        for (uint to = from; to <= a.length; to++) {
            if (b[from][to].val != (c[to] - c[from]).val) {
                println("bad");
            }
        }
    }
    println("completed");
}