#include <sstream>
#include <string>
#include <iostream>

template <typename T>
class LinkedList {
    struct Node { T v; Node* next; };
    Node* h;
    std::size_t sz;

    void appendNode(Node* n) {
        if (!h) { h = n; return; }
        Node* t = h;
        while (t->next) t = t->next;
        t->next = n;
    }

public:
    LinkedList() : h(nullptr), sz(0) {}
    ~LinkedList() {
        Node* p = h;
        while (p) { Node* d = p; p = p->next; delete d; }
    }

    void push_back(const T& val) {
        appendNode(new Node{val, nullptr});
        ++sz;
    }

    std::size_t size() const { return sz; }

    class Iterator {
        Node* n;
    public:
        Iterator(Node* node) : n(node) {}
        T& operator*() { return n->v; }
        const T& operator*() const { return n->v; }
        Iterator& operator++() { n = n->next; return *this; }
        bool operator!=(const Iterator& o) { return n != o.n; }
    };

    Iterator begin() { return Iterator(h); }
    Iterator end() { return Iterator(nullptr); }
    Iterator begin() const { return Iterator(h); }
    Iterator end() const { return Iterator(nullptr); }
};

template <typename T>
LinkedList<T> merge_sorted(const LinkedList<T>& left, const LinkedList<T>& right) {
    LinkedList<T> out;
    auto a = left.begin(), b = right.begin();
    while (a != left.end() && b != right.end()) {
        if (*a <= *b) { out.push_back(*a); ++a; }
        else { out.push_back(*b); ++b; }
    }
    for (; a != left.end(); ++a) out.push_back(*a);
    for (; b != right.end(); ++b) out.push_back(*b);
    return out;
}

int main() {
    LinkedList<int> seqOne, seqTwo;
    std::string s;
    std::getline(std::cin, s);
    { std::istringstream in(s); int x; while (in >> x) seqOne.push_back(x); }
    std::getline(std::cin, s);
    { std::istringstream in(s); int x; while (in >> x) seqTwo.push_back(x); }

    LinkedList<int> answer = merge_sorted(seqOne, seqTwo);
    bool sep = false;
    for (auto i = answer.begin(); i != answer.end(); ++i) {
        if (sep) std::cout << ' ';
        std::cout << *i;
        sep = true;
    }
    std::cout << '\n';
    return 0;
}
