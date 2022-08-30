#include "list.cpp"

struct DST {
    List<uint> nodes;
    List<ubyte> ranks;
    inline DST() {
        nodes = List<uint>();
        ranks = List<ubyte>();
    }
    inline DST(uint l) {
        nodes = List<uint>(Array<uint>::indexes(l));
        ranks = List<ubyte>(l);
        ranks.fill();
    }
    constexpr void release() const {
        nodes.release();
        ranks.release();
    }
    inline uint add() {
        nodes.add(nodes.size);
        ranks.add(0);
        return nodes.size - 1;
    }
    inline uint merge(uint a, uint b) const {
        if (ranks[a] < ranks[b]) {
            nodes.ptr()[a] = b;
            return b;
        }
        nodes.ptr()[b] = a;
        if (ranks[b] == ranks[a]) {
            ranks.ptr()[a]++;
        }
        return a;
    }
    uint find(uint a) const {
        uint cur = a;
        uint* n = nodes.ptr();
        for (;n[cur] != cur; cur = n[cur]);
        uint ans = cur;
        cur = a;
        uint next;
        for (;cur != ans;) {
            next = n[cur];
            n[cur] = ans;
            cur = next;
        }
        return ans;
    }
};

inline std::ostream& operator<<(std::ostream& os, DST d) {
    os << d.nodes;
    return os;
}

namespace my {
RELF(Comm)
}