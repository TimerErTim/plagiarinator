#include <sstream>
#include <string>
#include <iostream>

using std::cin;
using std::cout;
using std::getline;
using std::istringstream;
using std::string;
using size_t = std::size_t;

template <typename T>
class LinkedList
{
    struct Node { T val; Node* nextPtr; Node(T v): val(v), nextPtr(nullptr) {} };
    Node* headNode;
    size_t length;

public:
    LinkedList(): headNode(nullptr), length(0) {}
    ~LinkedList() { clear(); }

    void clear() {
        while (headNode) { Node* t = headNode; headNode = headNode->nextPtr; delete t; }
        length = 0;
    }

    void push_back(const T& v) {
        Node* node = new Node(v);
        if (!headNode) headNode = node;
        else {
            Node* p = headNode;
            while (p->nextPtr) p = p->nextPtr;
            p->nextPtr = node;
        }
        ++length;
    }

    void emplace_back(const T& v) { push_back(v); }

    size_t size() const { return length; }

    class Iterator {
        Node* pos;
    public:
        Iterator(Node* n): pos(n) {}
        T& operator*() { return pos->val; }
        const T& operator*() const { return pos->val; }
        Iterator& operator++() { pos = pos->nextPtr; return *this; }
        Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }
        bool operator!=(const Iterator& o) { return pos != o.pos; }
    };

    Iterator begin() { return Iterator(headNode); }
    Iterator end() { return Iterator(nullptr); }
    Iterator begin() const { return Iterator(headNode); }
    Iterator end() const { return Iterator(nullptr); }
};

template <typename T>
LinkedList<T> merge_sorted(const LinkedList<T>& L, const LinkedList<T>& R)
{
    LinkedList<T> out;
    auto i = L.begin(), j = R.begin();
    while (i != L.end() && j != R.end()) {
        if (*i <= *j) { out.emplace_back(*i); ++i; }
        else { out.push_back(*j); ++j; }
    }
    for (; i != L.end(); i++) out.emplace_back(*i);
    for (; j != R.end(); j++) out.push_back(*j);
    return out;
}

LinkedList<int> parseInts(const string& line)
{
    LinkedList<int> lst;
    istringstream iss(line);
    int v;
    while (iss >> v) lst.emplace_back(v);
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
    cout << '\n';
    return 0;
}
