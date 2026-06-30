#include <string>
#include <sstream>
#include <iostream>

template <typename T>
class LinkedList {
    struct Node { T v; Node* next; };
    Node* head_;
    std::size_t len_;

    void appendNode(Node* node) {
        if (!head_) { head_ = node; return; }
        Node* trail = head_;
        while (trail->next) trail = trail->next;
        trail->next = node;
    }

public:
    LinkedList() : head_(nullptr), len_(0) {}
    ~LinkedList() {
        Node* p = head_;
        while (p) { Node* d = p; p = p->next; delete d; }
    }

    void push_back(const T& val) {
        appendNode(new Node{val, nullptr});
        ++len_;
    }

    void emplace_back(const T& val) { push_back(val); }

    std::size_t size() const { return len_; }

    class Iterator {
        Node* node_;
    public:
        Iterator(Node* node) : node_(node) {}
        T& operator*() { return node_->v; }
        const T& operator*() const { return node_->v; }
        Iterator& operator++() { node_ = node_->next; return *this; }
        Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }
        bool operator!=(const Iterator& o) { return node_ != o.node_; }
    };

    Iterator begin() { return Iterator(head_); }
    Iterator end() { return Iterator(nullptr); }
    Iterator begin() const { return Iterator(head_); }
    Iterator end() const { return Iterator(nullptr); }
};

template <typename T>
LinkedList<T> merge_sorted(const LinkedList<T>& A, const LinkedList<T>& B) {
    LinkedList<T> C;
    auto a = A.begin(), b = B.begin();
    while (a != A.end() && b != B.end()) {
        if (*a <= *b) { C.emplace_back(*a); ++a; }
        else { C.emplace_back(*b); ++b; }
    }
    for (; a != A.end(); a++) C.emplace_back(*a);
    for (; b != B.end(); b++) C.emplace_back(*b);
    return C;
}

int main() {
    LinkedList<int> L1, L2;
    std::string s;
    std::getline(std::cin, s);
    { std::istringstream in(s); int x; while (in >> x) L1.emplace_back(x); }
    std::getline(std::cin, s);
    { std::istringstream in(s); int x; while (in >> x) L2.emplace_back(x); }

    LinkedList<int> M = merge_sorted(L1, L2);
    bool needSpace = false;
    for (auto i = M.begin(); i != M.end(); i++) {
        if (needSpace) std::cout << ' ';
        std::cout << *i;
        needSpace = true;
    }
    std::cout << std::endl;
    return 0;
}
