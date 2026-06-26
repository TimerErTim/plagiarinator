#include <iostream>
#include <string>
#include <vector>
// cleaned up version

class IntBST {
    struct Node { int x; Node* lo; Node* hi; Node(int v): x(v), lo(0), hi(0) {} };
    Node* rt;

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
        dump(n->lo, o); o.push_back(n->x); dump(n->hi, o);
    }

    Node* low(Node* n) const { while (n && n->lo) n = n->lo; return n; }

public:
    IntBST(): rt(0) {}
    ~IntBST() { kill(rt); }
    void insert(int v) { rt = put(rt, v); }
    bool contains(int v) const { return got(rt, v); }
    void inorder(std::vector<int>& o) const { dump(rt, o); }
    int& min() { return low(rt)->x; }
    const int& min() const { return low(rt)->x; }
    bool empty() const { return !rt; }
};

int main() {
    IntBST bst;
    std::string word;
    while (std::cin >> word) {
        if (word == "DONE") break;
        bst.insert(std::stoi(word));
    }
    while (std::cin >> word) {
        if (word == "DONE") break;
        if (word == "CONTAINS") {
            int q; std::cin >> q;
            std::cout << (bst.contains(q) ? "YES" : "NO") << std::endl;
        } else if (word == "MIN") {
            if (bst.empty()) std::cout << "EMPTY\n";
            else std::cout << bst.min() << std::endl;
        } else if (word == "PRINT") {
            std::vector<int> out; bst.inorder(out);
            for (std::size_t i = 0; i < out.size(); ++i) {
                if (i) std::cout << ' ';
                std::cout << out[i];
            }
            std::cout << std::endl;
        }
    }
    return 0;
}
