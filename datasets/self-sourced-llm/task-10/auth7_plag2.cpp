#include <iostream>
#include <string>
#include <vector>

class IntBST
{
    struct N { int v; N* a; N* b; N(int x):v(x),a(0),b(0){} };
    N* r;

    void del(N* n)
    {
        if (!n) return;
        del(n->a);
        del(n->b);
        delete n;
    }

    N* add(N* n, int x)
    {
        if (!n) return new N(x);
        if (x < n->v) n->a = add(n->a, x);
        else if (x > n->v) n->b = add(n->b, x);
        return n;
    }

    bool probe(const N* n, int x) const
    {
        if (!n) return false;
        if (x == n->v) return true;
        if (x < n->v) return probe(n->a, x);
        return probe(n->b, x);
    }

    void lst(const N* n, std::vector<int>& o) const
    {
        if (!n) return;
        lst(n->a, o);
        o.push_back(n->v);
        lst(n->b, o);
    }

    N* mn(N* n) const
    {
        while (n && n->a) n = n->a;
        return n;
    }

public:
    IntBST(): r(0) {}
    ~IntBST() { del(r); }
    void insert(int x) { r = add(r, x); }
    bool contains(int x) const { return probe(r, x); }
    void inorder(std::vector<int>& o) const { lst(r, o); }
    int& min() { return mn(r)->v; }
    const int& min() const { return mn(r)->v; }
    bool empty() const { return !r; }
};

int main()
{
    IntBST t;
    std::string cmd;
    while (std::cin >> cmd)
    {
        if (cmd == "DONE") break;
        t.insert(std::stoi(cmd));
    }
    while (std::cin >> cmd)
    {
        if (cmd == "DONE") break;
        if (cmd == "CONTAINS")
        {
            int x; std::cin >> x;
            if (t.contains(x)) std::cout << "YES\n";
            else std::cout << "NO\n";
        }
        else if (cmd == "MIN")
        {
            if (t.empty()) std::cout << "EMPTY\n";
            else std::cout << t.min() << '\n';
        }
        else if (cmd == "PRINT")
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
