#include <string>
#include <iostream>
#include <sstream>
// from lab partner

template <typename T>
class LinkedList {
public:
    class Iterator;

private:
    struct Node {
        T value;
        Node* next;
    };
    Node* head;
    std::size_t n;

public:
    LinkedList() : head(nullptr), n(0) {}

    ~LinkedList() {
        while (head) {
            Node* t = head;
            head = head->next;
            delete t;
        }
    }

    void push_back(const T& v) {
        Node* node = new Node{v, nullptr};
        if (!head) head = node;
        else {
            Node* p = head;
            while (p->next) p = p->next;
            p->next = node;
        }
        ++n;
    }

    std::size_t size() const { return n; }

    class Iterator {
        Node* cur;
        friend class LinkedList;
        Iterator(Node* p) : cur(p) {}
    public:
        T& operator*() { return cur->value; }
        const T& operator*() const { return cur->value; }
        Iterator& operator++() { cur = cur->next; return *this; }
        bool operator!=(const Iterator& o) const { return cur != o.cur; }
    };

    Iterator begin() { return Iterator(head); }
    Iterator end() { return Iterator(nullptr); }
    Iterator begin() const { return Iterator(head); }
    Iterator end() const { return Iterator(nullptr); }
};

template <typename T>
LinkedList<T> merge_sorted(const LinkedList<T>& left, const LinkedList<T>& right) {
    LinkedList<T> dst;
    auto li = left.begin(), ri = right.begin();
    while (li != left.end() && ri != right.end()) {
        if (*li <= *ri) { dst.push_back(*li); ++li; }
        else { dst.push_back(*ri); ++ri; }
    }
    while (li != left.end()) { dst.push_back(*li); ++li; }
    while (ri != right.end()) { dst.push_back(*ri); ++ri; }
    return dst;
}

static void fillFromLine(LinkedList<int>& list, const std::string& line) {
    std::istringstream ss(line);
    int v;
    while (ss >> v)
        list.push_back(v);
}

int main() {
    std::string lineA, lineB;
    std::getline(std::cin, lineA);
    std::getline(std::cin, lineB);

    LinkedList<int> listA, listB;
    fillFromLine(listA, lineA);
    fillFromLine(listB, lineB);

    LinkedList<int> res = merge_sorted(listA, listB);
    bool first = true;
    for (auto it = res.begin(); it != res.end(); ++it) {
        if (!first) std::cout << ' ';
        std::cout << *it;
        first = false;
    }
    std::cout << '\n';
    return 0;
}
