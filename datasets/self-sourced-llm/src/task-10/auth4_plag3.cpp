#include <vector>
#include <iostream>
#include <string>
// TODO: refactor later

class IntBST {
    struct Node { int val; Node* l; Node* r; Node(int v): val(v), l(0), r(0) {} };
    Node* root;

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
        seq(n->l, o); o.push_back(n->val); seq(n->r, o);
    }

    Node* smallest(Node* n) const {
        while (n && n->l) n = n->l;
        return n;
    }

public:
    IntBST() : root(0) {}
    ~IntBST() { clear(root); }

    void insert(int v) { root = add(root, v); }
    bool contains(int v) const { return find(root, v); }
    void inorder(std::vector<int>& o) const { seq(root, o); }
    int& min() { return smallest(root)->val; }
    const int& min() const { return smallest(root)->val; }
    bool empty() const { return !root; }
};

int main() {
    IntBST hold;
    std::string s;
    while (std::cin >> s && s != "DONE")
        hold.insert(std::stoi(s));

    while (std::cin >> s) {
        if (s == "DONE") break;
        if (s == "CONTAINS") {
            int x; std::cin >> x;
            std::cout << (hold.contains(x) ? "YES" : "NO") << std::endl;
        } else if (s == "MIN") {
            std::cout << (hold.empty() ? "EMPTY" : std::to_string(hold.min())) << std::endl;
        } else if (s == "PRINT") {
            std::vector<int> v; hold.inorder(v);
            for (size_t i = 0; i < v.size(); ++i) {
                if (i) std::cout << ' ';
                std::cout << v[i];
            }
            std::cout << std::endl;
        }
    }
    return 0;
}
