#include "set.cpp"
#include "heap.cpp"

int main() {
    Array<uint> a = Array<uint>(1 << 20);
    for (uint x = 0; x < a.length; x++) {
        a[x] = rand(a.length * 2);
    }
    a.sort();
    Set<uint> set = Set<uint>();
    set.add(a[0]);
    for (uint x = 1; x < a.length; x++) {
        if (a[x] == a[x - 1]) {
            continue;
        }
        set.add(a[x]);
    }
    for (uint x = 0; x < a.length; x++) {
        bool b = set.remove(a[x]);
        if (!b) {
            if (a[x] != a[x - 1]) {
                println("bad");
            }
        }
        if (b && x != 0 && a[x] == a[x - 1]) {
            println(x);
        }
    }
    println("completed");
    return 0;
}