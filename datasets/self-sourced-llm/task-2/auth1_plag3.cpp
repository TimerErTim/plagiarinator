#include <string>
#include <iostream>
#include <sstream>
// from lab partner

template <typename T>
class LinkedList {
    struct Node {
        T value;
        Node* next;
        Node(const T& v) : value(v), next(nullptr) {}
    };
    Node* ptr_;
    std::size_t num_;

public:
    LinkedList() : ptr_(nullptr), num_(0) {}
    ~LinkedList() {
        while (ptr_) {
            Node* tmp = ptr_;
            ptr_ = ptr_->next;
            delete tmp;
        }
    }

    void push_back(const T& v) {
        Node* n = new Node(v);
        if (!ptr_) ptr_ = n;
        else {
            Node* cur = ptr_;
            while (cur->next) cur = cur->next;
            cur->next = n;
        }
        ++num_;
    }

    std::size_t size() const { return num_; }

    class Iterator {
        Node* node_;
    public:
        explicit Iterator(Node* n) : node_(n) {}
        T& operator*() const { return node_->value; }
        Iterator& operator++() { node_ = node_->next; return *this; }
        bool operator!=(const Iterator& o) const { return node_ != o.node_; }
    };

    Iterator begin() const { return Iterator(ptr_); }
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
    LinkedList<int> lhs = readLine(std::cin);
    LinkedList<int> rhs = readLine(std::cin);
    LinkedList<int> res = merge_sorted(lhs, rhs);
    bool first = true;
    for (auto it = res.begin(); it != res.end(); ++it) {
        if (!first) std::cout << ' ';
        std::cout << *it;
        first = false;
    }
    std::cout << '\n';
    return 0;
}
