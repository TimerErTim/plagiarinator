#include <iostream>
#include <string>
#include <sstream>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::getline;
using std::istringstream;
using std::string;
using size_t = std::size_t;

template <typename T>
class LinkedList
{
    struct Node { T val; Node* nxt; Node(T v): val(v), nxt(nullptr) {} };
    Node* hd;
    size_t sz;

    void attachTail(Node* node)
    {
        if (!hd) hd = node;
        else
        {
            Node* p = hd;
            while (p->nxt) p = p->nxt;
            p->nxt = node;
        }
    }

public:
    LinkedList(): hd(nullptr), sz(0) {}
    ~LinkedList() { clear(); }

    void clear()
    {
        while (hd) { Node* t = hd; hd = hd->nxt; delete t; }
        sz = 0;
    }

    void push_back(const T& v)
    {
        attachTail(new Node(v));
        sz++;
    }

    size_t size() const { return sz; }

    class Iterator
    {
        Node* cur;
    public:
        Iterator(Node* n): cur(n) {}
        T& operator*() { return cur->val; }
        const T& operator*() const { return cur->val; }
        Iterator& operator++() { cur = cur->nxt; return *this; }
        bool operator!=(const Iterator& o) { return cur != o.cur; }
    };

    Iterator begin() { return Iterator(hd); }
    Iterator end() { return Iterator(nullptr); }
    Iterator begin() const { return Iterator(hd); }
    Iterator end() const { return Iterator(nullptr); }
};

template <typename T>
LinkedList<T> merge_sorted(const LinkedList<T>& L, const LinkedList<T>& R)
{
    LinkedList<T> out;
    auto i = L.begin(), j = R.begin();
    while (i != L.end() && j != R.end())
    {
        if (*i <= *j) { out.push_back(*i); ++i; }
        else { out.push_back(*j); ++j; }
    }
    while (i != L.end()) { out.push_back(*i); ++i; }
    while (j != R.end()) { out.push_back(*j); ++j; }
    return out;
}

LinkedList<int> parseInts(const string& line)
{
    LinkedList<int> lst;
    istringstream iss(line);
    std::vector<int> tmp;
    int v;
    while (iss >> v) tmp.push_back(v);
    for (size_t idx = 0; idx < tmp.size(); ++idx)
        lst.push_back(tmp[idx]);
    return lst;
}

int main()
{
    string l2, l1;
    getline(cin, l2);
    getline(cin, l1);
    auto merged = merge_sorted(parseInts(l1), parseInts(l2));
    bool sep = false;
    for (auto it = merged.begin(); it != merged.end(); ++it)
    {
        if (sep) cout << ' ';
        cout << *it;
        sep = true;
    }
    cout << endl;
    return 0;
}
