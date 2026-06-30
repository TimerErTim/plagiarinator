#include <vector>
#include <iostream>
#include <sstream>
#include <string>

class IntBST {
    struct Node { int x; Node* lo; Node* hi; Node(int v): x(v), lo(0), hi(0) {} };
    Node* rtk;

    void kill(Node* n) { if (n) { kill(n->lo); kill(n->hi); delete n; } }

    Node* put(Node* n, int v) {
        if (!n) return new Node(v);
        if (v < n->x) n->lo = put(n->lo, v);
        else if (v > n->x) n->hi = put(n->hi, v);
        return n;
    }

    bool got(const Node* n, int v) const {
        if (!n) return false;
        if (v == n->x) return true;
        return v < n->x ? got(n->lo, v) : got(n->hi, v);
    }

    void dump(const Node* n, std::vector<int>& o) const {
        if (!n) return;
        dump(n->lo, o); o.emplace_back(n->x); dump(n->hi, o);
    }

    Node* low(Node* n) const { while (n && n->lo) n = n->lo; return n; }

public:
    IntBST(): rtk(0) {}
    ~IntBST() { kill(rtk); }
    void insert(int v) { rtk = put(rtk, v); }
    bool contains(int v) const { return got(rtk, v); }
    void inorder(std::vector<int>& o) const { dump(rtk, o); }
    int& min() { return low(rtk)->x; }
    const int& min() const { return low(rtk)->x; }
    bool empty() const { return !rtk; }
};

int main() {
    IntBST tree;
    std::string tok;
    while (std::cin >> tok) {
        if (tok == "DONE") break;
        tree.insert(std::stoi(tok));
    }
    while (std::cin >> tok) {
        if (tok == "DONE") break;
        if (tok == "CONTAINS") {
            int q; std::cin >> q;
            std::cout << (tree.contains(q) ? "YES" : "NO") << std::endl;
        } else if (tok == "MIN") {
            if (tree.empty()) std::cout << "EMPTY" << std::endl;
            else std::cout << tree.min() << std::endl;
        } else if (tok == "PRINT") {
            std::vector<int> vals; tree.inorder(vals);
            for (std::size_t i = 0; i < vals.size(); i++) {
                if (i) std::cout << ' ';
                std::cout << vals[i];
            }
            std::cout << std::endl;
        }
    }
    return 0;
}
