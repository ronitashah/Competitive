#include "list.cpp"
#include "heap.cpp"

int main() {
    Array<uint> a = {1, 5, 3, 8, 4, 9, 2};
    Heap<uint> h = Heap<uint>();
    for (uint x = 0; x < a.length; x++) {
        h.add(a[x]);
    }
    println(h);
}