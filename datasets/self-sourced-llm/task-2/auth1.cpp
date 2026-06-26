#include <iostream>
#include <sstream>
#include <string>

template <typename T>
class LinkedList {
    struct Node {
        T value;
        Node* next;
        Node(const T& v) : value(v), next(nullptr) {}
    };
    Node* head_;
    std::size_t count_;

public:
    LinkedList() : head_(nullptr), count_(0) {}
    ~LinkedList() {
        while (head_) {
            Node* tmp = head_;
            head_ = head_->next;
            delete tmp;
        }
    }

    void push_back(const T& v) {
        Node* n = new Node(v);
        if (!head_) head_ = n;
        else {
            Node* cur = head_;
            while (cur->next) cur = cur->next;
            cur->next = n;
        }
        ++count_;
    }

    std::size_t size() const { return count_; }

    class Iterator {
        Node* node_;
    public:
        explicit Iterator(Node* n) : node_(n) {}
        T& operator*() const { return node_->value; }
        Iterator& operator++() { node_ = node_->next; return *this; }
        bool operator!=(const Iterator& o) const { return node_ != o.node_; }
    };

    Iterator begin() const { return Iterator(head_); }
    Iterator end() const { return Iterator(nullptr); }
};

template <typename T>
LinkedList<T> merge_sorted(const LinkedList<T>& a, const LinkedList<T>& b) {
    LinkedList<T> result;
    auto ia = a.begin(), ib = b.begin();
    auto ea = a.end(), eb = b.end();
    while (ia != ea && ib != eb) {
        if (*ia <= *ib) { result.push_back(*ia); ++ia; }
        else { result.push_back(*ib); ++ib; }
    }
    while (ia != ea) { result.push_back(*ia); ++ia; }
    while (ib != eb) { result.push_back(*ib); ++ib; }
    return result;
}

static LinkedList<int> readLine(std::istream& in) {
    LinkedList<int> list;
    std::string line;
    if (!std::getline(in, line)) return list;
    std::istringstream ss(line);
    int x;
    while (ss >> x) list.push_back(x);
    return list;
}

int main() {
    LinkedList<int> left = readLine(std::cin);
    LinkedList<int> right = readLine(std::cin);
    LinkedList<int> merged = merge_sorted(left, right);
    bool first = true;
    for (auto it = merged.begin(); it != merged.end(); ++it) {
        if (!first) std::cout << ' ';
        std::cout << *it;
        first = false;
    }
    std::cout << '\n';
    return 0;
}
