#include <string>
#include <sstream>
#include <iostream>
// adapted from classmate submission

template <typename T>
class LinkedList {
    struct Node { T v; Node* next; };
    Node* h;
    std::size_t sz;

    void appendNode(Node* n) {
        if (!h)
{ h = n; return; }
        Node* t = h;
        while (t->next) t = t->next;
        t->next = n;
    }

public:
    LinkedList() : h(nullptr), sz(0) {}
    ~LinkedList() {
        Node* p = h;
        while (p)
{ Node* d = p; p = p->next; delete d; }
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
LinkedList<T> merge_sorted(const LinkedList<T>& A, const LinkedList<T>& B) {
    LinkedList<T> C;
    auto a = A.begin(), b = B.begin();
    while (a != A.end() && b != B.end()) {
        if (*a <= *b)
{ C.push_back(*a); ++a; }
        else { C.push_back(*b); ++b; }
    }
    for (; a != A.end(); ++a) C.push_back(*a);
    for (; b != B.end(); ++b) C.push_back(*b);
    return C;
}

int main()
{
    LinkedList<int> listOne, listTwo;
    std::string s;
    std::getline(std::cin, s);
    { std::istringstream in(s); int x; while (in >> x) listOne.push_back(x); }
    std::getline(std::cin, s);
    { std::istringstream in(s); int x; while (in >> x) listTwo.push_back(x); }

    LinkedList<int> M = merge_sorted(listOne, listTwo);
    bool sep = false;
    for (auto i = M.begin(); i != M.end(); ++i) {
        if (sep) std::cout << ' ';
        std::cout << *i;
        sep = true;
    }
    std::cout << '\n';
    return 0;
}
