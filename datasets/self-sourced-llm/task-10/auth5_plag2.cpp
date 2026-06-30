#include <iostream>
#include <string>
#include <vector>

class IntBST
{
    struct Node { int x; Node* lo; Node* hi; Node(int v): x(v), lo(0), hi(0) {} };
    Node* rt;

    void kill(Node* n)
    {
        if (!n) return;
        kill(n->lo);
        kill(n->hi);
        delete n;
    }

    Node* put(Node* n, int v)
    {
        if (!n) return new Node(v);
        if (v < n->x) n->lo = put(n->lo, v);
        else if (v > n->x) n->hi = put(n->hi, v);
        return n;
    }

    bool holds(const Node* n, int v) const
    {
        if (!n) return false;
        if (v == n->x) return true;
        if (v < n->x) return holds(n->lo, v);
        return holds(n->hi, v);
    }

    void dump(const Node* n, std::vector<int>& o) const
    {
        if (!n) return;
        dump(n->lo, o);
        o.push_back(n->x);
        dump(n->hi, o);
    }

    Node* low(Node* n) const
    {
        while (n && n->lo) n = n->lo;
        return n;
    }

public:
    IntBST(): rt(0) {}
    ~IntBST() { kill(rt); }
    void insert(int v) { rt = put(rt, v); }
    bool contains(int v) const { return holds(rt, v); }
    void inorder(std::vector<int>& o) const { dump(rt, o); }
    int& min() { return low(rt)->x; }
    const int& min() const { return low(rt)->x; }
    bool empty() const { return !rt; }
};

int main()
{
    IntBST tree;
    std::string tok;
    while (std::cin >> tok)
    {
        if (tok == "DONE") break;
        tree.insert(std::stoi(tok));
    }
    while (std::cin >> tok)
    {
        if (tok == "DONE") break;
        if (tok == "CONTAINS")
        {
            int q; std::cin >> q;
            if (tree.contains(q)) std::cout << "YES\n";
            else std::cout << "NO\n";
        }
        else if (tok == "MIN")
        {
            if (tree.empty()) std::cout << "EMPTY\n";
            else std::cout << tree.min() << '\n';
        }
        else if (tok == "PRINT")
        {
            std::vector<int> vals;
            tree.inorder(vals);
            for (std::size_t j = 0; j < vals.size(); ++j)
            {
                if (j) std::cout << ' ';
                std::cout << vals[j];
            }
            std::cout << '\n';
        }
    }
    return 0;
}
