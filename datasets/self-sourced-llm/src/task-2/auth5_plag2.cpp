#include <iostream>
#include <sstream>
#include <string>

template <typename T>
class LinkedList
{
    struct Node { T v; Node* next; };
    Node* h;
    std::size_t sz;

public:
    LinkedList() : h(nullptr), sz(0) {}
    ~LinkedList()
    {
        Node* p = h;
        while (p) { Node* d = p; p = p->next; delete d; }
    }

    void push_back(const T& val)
    {
        Node* n = new Node{val, nullptr};
        if (!h) { h = n; }
        else
        {
            Node* t = h;
            while (t->next) t = t->next;
            t->next = n;
        }
        ++sz;
    }

    std::size_t size() const { return sz; }

    class Iterator
    {
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
LinkedList<T> merge_sorted(const LinkedList<T>& A, const LinkedList<T>& B)
{
    LinkedList<T> C;
    auto a = A.begin(), b = B.begin();
    while (a != A.end() && b != B.end())
    {
        if (*a <= *b) { C.push_back(*a); ++a; }
        else { C.push_back(*b); ++b; }
    }
    for (; a != A.end(); ++a) C.push_back(*a);
    for (; b != B.end(); ++b) C.push_back(*b);
    return C;
}

static void fillListFromLine(LinkedList<int>& list, const std::string& line)
{
    std::istringstream in(line);
    int x;
    while (in >> x) list.push_back(x);
}

int main()
{
    LinkedList<int> L1, L2;
    std::string s;
    std::getline(std::cin, s);
    fillListFromLine(L1, s);
    std::getline(std::cin, s);
    fillListFromLine(L2, s);

    LinkedList<int> M = merge_sorted(L1, L2);
    for (std::size_t k = 0; k < M.size(); ++k)
    {
        if (k > 0) std::cout << ' ';
        auto it = M.begin();
        for (std::size_t j = 0; j < k; ++j) ++it;
        std::cout << *it;
    }
    std::cout << '\n';
    return 0;
}
