#include <string>
#include <iostream>
#include <sstream>
// from lab partner

template <typename T>
class LinkedList {
    struct Node { T data; Node* nxt; Node(T d): data(d), nxt(0) {} };
    Node* front;
    std::size_t cnt;

public:
    LinkedList(): front(0), cnt(0) {}
    ~LinkedList() { while (front) { Node* t = front; front = front->nxt; delete t; } }

    void push_back(const T& x) {
        Node* nn = new Node(x);
        if (!front) front = nn;
        else {
            for (Node* p = front; ; p = p->nxt)
                if (!p->nxt) { p->nxt = nn; break; }
        }
        cnt++;
    }

    std::size_t size() const { return cnt; }

    class Iterator {
        Node* p;
    public:
        Iterator(Node* n): p(n) {}
        T& operator*() { return p->data; }
        const T& operator*() const { return p->data; }
        Iterator& operator++() { p = p->nxt; return *this; }
        bool operator!=(const Iterator& x) { return p != x.p; }
    };

    Iterator begin() { return Iterator(front); }
    Iterator end() { return Iterator(0); }
    Iterator begin() const { return Iterator(front); }
    Iterator end() const { return Iterator(0); }
};

template <typename T>
LinkedList<T> merge_sorted(const LinkedList<T>& u, const LinkedList<T>& v) {
    LinkedList<T> w;
    auto i = u.begin(), j = v.begin();
    while (i != u.end() && j != v.end()) {
        if (*i <= *j) { w.push_back(*i); ++i; }
        else { w.push_back(*j); ++j; }
    }
    while (i != u.end()) { w.push_back(*i); ++i; }
    while (j != v.end()) { w.push_back(*j); ++j; }
    return w;
}

int main() {
    LinkedList<int> a, b;
    std::string ln;
    std::getline(std::cin, ln);
    { std::istringstream s(ln); int v; while (s >> v) a.push_back(v); }
    std::getline(std::cin, ln);
    { std::istringstream s(ln); int v; while (s >> v) b.push_back(v); }
    LinkedList<int> m = merge_sorted(a, b);
    bool first = true;
    for (auto p = m.begin(); p != m.end(); ++p) {
        if (!first) std::cout << ' ';
        std::cout << *p;
        first = false;
    }
    std::cout << "\n";
    return 0;
}
