#include <sstream>
#include <string>
#include <iostream>

template <typename T>
class LinkedList {
    struct Node { T data; Node* nxt; Node(T d): data(d), nxt(0) {} };
    Node* head;
    std::size_t num;

public:
    LinkedList(): head(0), num(0) {}
    ~LinkedList() { while (head) { Node* t = head; head = head->nxt; delete t; } }

    void push_back(const T& x) {
        Node* nn = new Node(x);
        if (!head) head = nn;
        else {
            for (Node* p = head; ; p = p->nxt)
                if (!p->nxt) { p->nxt = nn; break; }
        }
        num++;
    }

    void emplace_back(const T& x) { push_back(x); }

    std::size_t size() const { return num; }

    class Iterator {
        Node* p;
    public:
        Iterator(Node* n): p(n) {}
        T& operator*() { return p->data; }
        const T& operator*() const { return p->data; }
        Iterator& operator++() { p = p->nxt; return *this; }
        Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }
        bool operator!=(const Iterator& x) { return p != x.p; }
    };

    Iterator begin() { return Iterator(head); }
    Iterator end() { return Iterator(0); }
    Iterator begin() const { return Iterator(head); }
    Iterator end() const { return Iterator(0); }
};

template <typename T>
LinkedList<T> merge_sorted(const LinkedList<T>& u, const LinkedList<T>& v) {
    LinkedList<T> w;
    auto pa = u.begin(), pb = v.begin();
    while (pa != u.end() && pb != v.end()) {
        if (*pa <= *pb) { w.emplace_back(*pa); pa++; }
        else { w.emplace_back(*pb); pb++; }
    }
    while (pa != u.end()) { w.emplace_back(*pa); ++pa; }
    while (pb != v.end()) { w.emplace_back(*pb); ++pb; }
    return w;
}

int main() {
    LinkedList<int> a, b;
    std::string ln;
    std::getline(std::cin, ln);
    { std::istringstream s(ln); int val; while (s >> val) a.emplace_back(val); }
    std::getline(std::cin, ln);
    { std::istringstream s(ln); int val; while (s >> val) b.emplace_back(val); }
    LinkedList<int> m = merge_sorted(a, b);
    bool gap = true;
    for (auto p = m.begin(); p != m.end(); p++) {
        if (!gap) std::cout << ' ';
        std::cout << *p;
        gap = false;
    }
    std::cout << std::endl;
    return 0;
}
