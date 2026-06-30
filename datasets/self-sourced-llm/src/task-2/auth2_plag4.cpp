#include <sstream>
#include <iostream>
#include <string>

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
    Node* headLink;
    size_t sz;

public:
    LinkedList(): headLink(nullptr), sz(0) {}
    ~LinkedList() { clear(); }

    void clear() {
        while (headLink) { Node* t = headLink; headLink = headLink->nxt; delete t; }
        sz = 0;
    }

    void push_back(const T& v) {
        Node* node = new Node(v);
        if (!headLink) headLink = node;
        else {
            Node* p = headLink;
            while (p->nxt) p = p->nxt;
            p->nxt = node;
        }
        sz++;
    }

    size_t size() const { return sz; }

    class Iterator {
        Node* cur;
    public:
        Iterator(Node* n): cur(n) {}
        T& operator*() { return cur->val; }
        const T& operator*() const { return cur->val; }
        Iterator& operator++() { cur = cur->nxt; return *this; }
        bool operator!=(const Iterator& o) { return cur != o.cur; }
    };

    Iterator begin() { return Iterator(headLink); }
    Iterator end() { return Iterator(nullptr); }
    Iterator begin() const { return Iterator(headLink); }
    Iterator end() const { return Iterator(nullptr); }
};

template <typename T>
LinkedList<T> merge_sorted(const LinkedList<T>& L, const LinkedList<T>& R)
{
    LinkedList<T> out;
    auto i = L.begin(), j = R.begin();
    while (i != L.end() && j != R.end()) {
        if (*i <= *j) { out.push_back(*i); ++i; }
        else { out.push_back(*j); ++j; }
    }
    for (; i != L.end(); ++i) out.push_back(*i);
    for (; j != R.end(); ++j) out.push_back(*j);
    return out;
}

LinkedList<int> parseInts(const string& line)
{
    LinkedList<int> lst;
    istringstream iss(line);
    int v;
    while (iss >> v) lst.push_back(v);
    return lst;
}

int main()
{
    string l1, l2;
    getline(cin, l1);
    getline(cin, l2);
    auto merged = merge_sorted(parseInts(l1), parseInts(l2));
    bool sep = false;
    for (auto it = merged.begin(); it != merged.end(); ++it) {
        if (sep) cout << ' ';
        cout << *it;
        sep = true;
    }
    cout << endl;
    return 0;
}
