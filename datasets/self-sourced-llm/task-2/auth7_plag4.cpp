#include <string>
#include <iostream>
#include <sstream>

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
LinkedList<T> merge_sorted(const LinkedList<T>& lhs, const LinkedList<T>& rhs) {
    LinkedList<T> out;
    auto i = lhs.begin(), j = rhs.begin();
    while (i != lhs.end() && j != rhs.end()) {
        if (*i <= *j) { out.push_back(*i); ++i; }
        else { out.push_back(*j); ++j; }
    }
    while (i != lhs.end()) { out.push_back(*i); ++i; }
    while (j != rhs.end()) { out.push_back(*j); ++j; }
    return out;
}

int main() {
    LinkedList<int> listOne, listTwo;
    std::string ln;
    std::getline(std::cin, ln);
    { std::istringstream s(ln); int v; while (s >> v) listOne.push_back(v); }
    std::getline(std::cin, ln);
    { std::istringstream s(ln); int v; while (s >> v) listTwo.push_back(v); }
    LinkedList<int> answer = merge_sorted(listOne, listTwo);
    bool first = true;
    for (auto p = answer.begin(); p != answer.end(); ++p) {
        if (!first) std::cout << ' ';
        std::cout << *p;
        first = false;
    }
    std::cout << "\n";
    return 0;
}
