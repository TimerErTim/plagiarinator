#include <iostream>
#include <string>
#include <vector>

class IntBST
{
    struct Node { int val; Node* l; Node* r; Node(int v): val(v), l(0), r(0) {} };
    Node* root;

    void clear(Node* n)
    {
        if (!n) return;
        clear(n->l);
        clear(n->r);
        delete n;
    }

    Node* add(Node* n, int v)
    {
        if (!n) return new Node(v);
        if (v < n->val) n->l = add(n->l, v);
        else if (v > n->val) n->r = add(n->r, v);
        return n;
    }

    bool seekVal(const Node* n, int v) const
    {
        if (!n) return false;
        if (v == n->val) return true;
        if (v < n->val) return seekVal(n->l, v);
        return seekVal(n->r, v);
    }

    void seq(const Node* n, std::vector<int>& o) const
    {
        if (!n) return;
        seq(n->l, o);
        o.push_back(n->val);
        seq(n->r, o);
    }

    Node* smallest(Node* n) const
    {
        while (n && n->l) n = n->l;
        return n;
    }

public:
    IntBST() : root(0) {}
    ~IntBST() { clear(root); }

    void insert(int v) { root = add(root, v); }
    bool contains(int v) const { return seekVal(root, v); }
    void inorder(std::vector<int>& o) const { seq(root, o); }
    int& min() { return smallest(root)->val; }
    const int& min() const { return smallest(root)->val; }
    bool empty() const { return !root; }
};

int main()
{
    IntBST t;
    std::string s;
    while (std::cin >> s)
    {
        if (s == "DONE") break;
        t.insert(std::stoi(s));
    }

    while (std::cin >> s)
    {
        if (s == "DONE") break;
        if (s == "CONTAINS")
        {
            int x; std::cin >> x;
            if (t.contains(x)) std::cout << "YES\n";
            else std::cout << "NO\n";
        }
        else if (s == "MIN")
        {
            if (t.empty()) std::cout << "EMPTY\n";
            else std::cout << t.min() << '\n';
        }
        else if (s == "PRINT")
        {
            std::vector<int> v;
            t.inorder(v);
            for (size_t j = 0; j < v.size(); ++j)
            {
                if (j) std::cout << ' ';
                std::cout << v[j];
            }
            std::cout << '\n';
        }
    }
    return 0;
}
