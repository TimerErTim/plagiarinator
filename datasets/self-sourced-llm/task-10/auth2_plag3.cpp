#include <vector>
#include <iostream>
#include <string>
// borrowed from roommate's homework

using std::cin;
using std::cout;
using std::endl;
using std::stoi;
using std::string;
using std::vector;
using size_t = std::size_t;

class IntBST
{
    struct Cell { int payload; Cell* west; Cell* east; Cell(int p): payload(p), west(0), east(0) {} };
    Cell* apex;

    void purge(Cell* c)
    {
        if (!c) return;
        vector<Cell*> order;
        vector<Cell*> pending;
        pending.push_back(c);
        while (!pending.empty())
        {
            Cell* cur = pending.back();
            pending.pop_back();
            order.push_back(cur);
            if (cur->west) pending.push_back(cur->west);
            if (cur->east) pending.push_back(cur->east);
        }
        for (size_t k = order.size(); k-- > 0;)
            delete order[k];
    }

    Cell* graft(Cell* c, int p)
    {
        if (!c) return new Cell(p);
        if (p < c->payload) c->west = graft(c->west, p);
        else if (p > c->payload) c->east = graft(c->east, p);
        return c;
    }

    bool scan(const Cell* c, int p) const
    {
        if (!c) return false;
        if (p == c->payload) return true;
        if (p < c->payload) return scan(c->west, p);
        return scan(c->east, p);
    }

    void emit(const Cell* c, vector<int>& buf) const
    {
        if (!c) return;
        emit(c->west, buf);
        buf.push_back(c->payload);
        emit(c->east, buf);
    }

    Cell* floor(Cell* c) const
    {
        while (c && c->west) c = c->west;
        return c;
    }

public:
    IntBST(): apex(0) {}
    ~IntBST() { purge(apex); }

    void insert(int x) { apex = graft(apex, x); }
    bool contains(int x) const { return scan(apex, x); }
    void inorder(vector<int>& out) const { emit(apex, out); }

    int& min() { return floor(apex)->payload; }
    const int& min() const { return floor(apex)->payload; }
    bool empty() const { return apex == 0; }
};

int main()
{
    IntBST holder;
    string piece;
    while (cin >> piece)
    {
        if (piece == "DONE") break;
        holder.insert(stoi(piece));
    }
    while (cin >> piece)
    {
        if (piece == "DONE") break;
        if (piece == "CONTAINS")
        {
            int x; cin >> x;
            cout << (holder.contains(x) ? "YES" : "NO") << endl;
        }
        else if (piece == "MIN")
        {
            if (holder.empty()) cout << "EMPTY" << endl;
            else cout << holder.min() << endl;
        }
        else if (piece == "PRINT")
        {
            vector<int> v;
            holder.inorder(v);
            for (size_t idx = 0; idx < v.size(); ++idx)
            {
                if (idx > 0) cout << ' ';
                cout << v[idx];
            }
            cout << endl;
        }
    }
    return 0;
}
