#include <iostream>
#include <string>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::stoi;
using std::string;
using std::vector;
using size_t = std::size_t;

class IntBST
{
    struct Node { int v; Node* L; Node* R; Node(int x): v(x), L(0), R(0) {} };
    Node* root;

    void freeTree(Node* n)
    {
        if (!n) return;
        freeTree(n->L);
        freeTree(n->R);
        delete n;
    }

    Node* ins(Node* n, int x)
    {
        if (!n) return new Node(x);
        if (x < n->v) n->L = ins(n->L, x);
        else if (x > n->v) n->R = ins(n->R, x);
        return n;
    }

    bool probe(const Node* n, int x) const
    {
        if (!n) return false;
        if (x == n->v) return true;
        if (x < n->v) return probe(n->L, x);
        return probe(n->R, x);
    }

    void walk(const Node* n, vector<int>& out) const
    {
        if (!n) return;
        walk(n->L, out);
        out.push_back(n->v);
        walk(n->R, out);
    }

    Node* leftmost(Node* n) const
    {
        while (n && n->L) n = n->L;
        return n;
    }

public:
    IntBST(): root(0) {}
    ~IntBST() { freeTree(root); }

    void insert(int x) { root = ins(root, x); }
    bool contains(int x) const { return probe(root, x); }
    void inorder(vector<int>& out) const { walk(root, out); }

    int& min() { return leftmost(root)->v; }
    const int& min() const { return leftmost(root)->v; }
    bool empty() const { return root == 0; }
};

int main()
{
    IntBST t;
    string cmd;
    while (cin >> cmd)
    {
        if (cmd == "DONE") break;
        t.insert(stoi(cmd));
    }
    while (cin >> cmd)
    {
        if (cmd == "DONE") break;
        if (cmd == "CONTAINS")
        {
            int x; cin >> x;
            if (t.contains(x)) cout << "YES\n";
            else cout << "NO\n";
        }
        else if (cmd == "MIN")
        {
            if (t.empty()) cout << "EMPTY\n";
            else cout << t.min() << '\n';
        }
        else if (cmd == "PRINT")
        {
            vector<int> v;
            t.inorder(v);
            for (size_t j = 0; j < v.size(); ++j)
            {
                if (j) cout << ' ';
                cout << v[j];
            }
            cout << '\n';
        }
    }
    return 0;
}
