#include <string>
#include <sstream>
#include <iostream>

template <typename T>
class LinkedList {
    struct Node {
        T value;
        Node* next;
        Node(const T& v) : value(v), next(nullptr) {}
    };
    Node* headPtr;
    std::size_t elemCount;

public:
    LinkedList() : headPtr(nullptr), elemCount(0) {}
    ~LinkedList() {
        while (headPtr) {
            Node* old = headPtr;
            headPtr = headPtr->next;
            delete old;
        }
    }

    void push_back(const T& v) {
        Node* n = new Node(v);
        if (!headPtr) headPtr = n;
        else {
            Node* walker = headPtr;
            while (walker->next) walker = walker->next;
            walker->next = n;
        }
        elemCount++;
    }

    void emplace_back(const T& v) { push_back(v); }

    std::size_t size() const { return elemCount; }

    class Iterator {
        Node* step;
    public:
        explicit Iterator(Node* n) : step(n) {}
        T& operator*() const { return step->value; }
        Iterator& operator++() { step = step->next; return *this; }
        bool operator!=(const Iterator& o) const { return step != o.step; }
    };

    Iterator begin() const { return Iterator(headPtr); }
    Iterator end() const { return Iterator(nullptr); }
};

template <typename T>
LinkedList<T> merge_sorted(const LinkedList<T>& a, const LinkedList<T>& b) {
    LinkedList<T> result;
    auto itA = a.begin(), itB = b.begin();
    auto endA = a.end(), endB = b.end();
    while (itA != endA && itB != endB) {
        if (*itA <= *itB) { result.emplace_back(*itA); ++itA; }
        else { result.push_back(*itB); ++itB; }
    }
    while (itA != endA) { result.emplace_back(*itA); ++itA; }
    while (itB != endB) { result.push_back(*itB); ++itB; }
    return result;
}

static LinkedList<int> readLine(std::istream& in) {
    LinkedList<int> list;
    std::string line;
    if (!std::getline(in, line)) return list;
    std::istringstream ss(line);
    int x;
    while (ss >> x) list.emplace_back(x);
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
    std::cout << std::endl;
    return 0;
}
