#include <sstream>
#include <string>
#include <iostream>

template <typename T>
class LinkedList {
public:
    class Iterator;

private:
    struct Node {
        T value;
        Node* next;
    };
    Node* front;
    std::size_t n;

public:
    LinkedList() : front(nullptr), n(0) {}

    ~LinkedList() {
        while (front) {
            Node* t = front;
            front = front->next;
            delete t;
        }
    }

    void push_back(const T& v) {
        Node* node = new Node{v, nullptr};
        if (!front) front = node;
        else {
            Node* p = front;
            while (p->next) p = p->next;
            p->next = node;
        }
        n++;
    }

    void emplace_back(const T& v) { push_back(v); }

    std::size_t size() const { return n; }

    class Iterator {
        Node* cur;
        friend class LinkedList;
        Iterator(Node* p) : cur(p) {}
    public:
        T& operator*() { return cur->value; }
        const T& operator*() const { return cur->value; }
        Iterator& operator++() { cur = cur->next; return *this; }
        Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }
        bool operator!=(const Iterator& o) const { return cur != o.cur; }
    };

    Iterator begin() { return Iterator(front); }
    Iterator end() { return Iterator(nullptr); }
    Iterator begin() const { return Iterator(front); }
    Iterator end() const { return Iterator(nullptr); }
};

template <typename T>
LinkedList<T> merge_sorted(const LinkedList<T>& left, const LinkedList<T>& right) {
    LinkedList<T> dst;
    auto li = left.begin(), ri = right.begin();
    while (li != left.end() && ri != right.end()) {
        if (*li <= *ri) { dst.emplace_back(*li); ++li; }
        else { dst.push_back(*ri); ++ri; }
    }
    for (; li != left.end(); ++li) dst.emplace_back(*li);
    for (; ri != right.end(); ri++) dst.push_back(*ri);
    return dst;
}

static void fillFromLine(LinkedList<int>& list, const std::string& line) {
    std::istringstream ss(line);
    int v;
    while (ss >> v)
        list.emplace_back(v);
}

int main() {
    std::string lineA, lineB;
    std::getline(std::cin, lineA);
    std::getline(std::cin, lineB);

    LinkedList<int> listA, listB;
    fillFromLine(listA, lineA);
    fillFromLine(listB, lineB);

    LinkedList<int> result = merge_sorted(listA, listB);
    bool first = true;
    for (auto it = result.begin(); it != result.end(); ++it) {
        if (!first) std::cout << ' ';
        std::cout << *it;
        first = false;
    }
    std::cout << '\n';
    return 0;
}
