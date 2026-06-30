#include <vector>
#include <iostream>
#include <string>

class IntBST {
    struct Node { int val; Node* l; Node* r; Node(int v): val(v), l(0), r(0) {} };
    Node* rootk;

    void clear(Node* n) {
        if (!n) return;
        clear(n->l); clear(n->r); delete n;
    }

    Node* add(Node* n, int v) {
        if (!n) return new Node(v);
        if (v < n->val) n->l = add(n->l, v);
        else if (v > n->val) n->r = add(n->r, v);
        return n;
    }

    bool find(const Node* n, int v) const {
        if (!n) return false;
        if (v == n->val) return true;
        return v < n->val ? find(n->l, v) : find(n->r, v);
    }

    void seq(const Node* n, std::vector<int>& o) const {
        if (!n) return;
        seq(n->l, o); o.emplace_back(n->val); seq(n->r, o);
    }

    Node* smallest(Node* n) const {
        while (n && n->l) n = n->l;
        return n;
    }

public:
    IntBST() : rootk(0) {}
    ~IntBST() { clear(rootk); }

    void insert(int v) { rootk = add(rootk, v); }
    bool contains(int v) const { return find(rootk, v); }
    void inorder(std::vector<int>& o) const { seq(rootk, o); }
    int& min() { return smallest(rootk)->val; }
    const int& min() const { return smallest(rootk)->val; }
    bool empty() const { return !rootk; }
};

int main() {
    IntBST t;
    std::string tok;
    while (std::cin >> tok && tok != "DONE")
        t.insert(std::stoi(tok));

    while (std::cin >> tok) {
        if (tok == "DONE") break;
        if (tok == "CONTAINS") {
            int x; std::cin >> x;
            std::cout << (t.contains(x) ? "YES" : "NO") << std::endl;
        } else if (tok == "MIN") {
            std::cout << (t.empty() ? "EMPTY" : std::to_string(t.min())) << std::endl;
        } else if (tok == "PRINT") {
            std::vector<int> v; t.inorder(v);
            for (size_t i = 0; i < v.size(); i++) {
                if (i) std::cout << ' ';
                std::cout << v[i];
            }
            std::cout << std::endl;
        }
    }
    return 0;
}
