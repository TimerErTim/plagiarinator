#include <iostream>
#include <sstream>
#include <string>
#include <utility>

template <typename T>
class LinkedList {
    struct Node {
        T val;
        Node* next;
    };

    Node* headPtr;
    std::size_t itemCount;

    static void destroyChain(Node* n) {
        while (n) {
            Node* save = n->next;
            delete n;
            n = save;
        }
    }

public:
    LinkedList() : headPtr(nullptr), itemCount(0) {}
    ~LinkedList() { destroyChain(headPtr); }

    void push_back(const T& value) {
        Node* node = new Node{value, nullptr};
        if (!headPtr) {
            headPtr = node;
        } else {
            Node* tail = headPtr;
            while (tail->next) tail = tail->next;
            tail->next = node;
        }
        itemCount++;
    }

    std::size_t size() const { return itemCount; }

    class Iterator {
        Node* current;
    public:
        Iterator(Node* n) : current(n) {}
        T& operator*() { return current->val; }
        const T& operator*() const { return current->val; }
        Iterator& operator++() { current = current->next; return *this; }
        bool operator!=(const Iterator& rhs) const { return current != rhs.current; }
    };

    Iterator begin() { return Iterator(headPtr); }
    Iterator end() { return Iterator(nullptr); }
    Iterator begin() const { return Iterator(headPtr); }
    Iterator end() const { return Iterator(nullptr); }
};

template <typename T>
LinkedList<T> merge_sorted(const LinkedList<T>& lhs, const LinkedList<T>& rhs) {
    LinkedList<T> output;
    auto leftIt = lhs.begin();
    auto rightIt = rhs.begin();

    while (leftIt != lhs.end() && rightIt != rhs.end()) {
        if (*leftIt <= *rightIt) {
            output.push_back(*leftIt);
            ++leftIt;
        } else {
            output.push_back(*rightIt);
            ++rightIt;
        }
    }
    for (; leftIt != lhs.end(); ++leftIt)
        output.push_back(*leftIt);
    for (; rightIt != rhs.end(); ++rightIt)
        output.push_back(*rightIt);
    return output;
}

static LinkedList<int> parseLine(std::istream& in) {
    LinkedList<int> list;
    std::string text;
    if (!std::getline(in, text))
        return list;
    std::istringstream stream(text);
    int number;
    while (stream >> number)
        list.push_back(number);
    return list;
}

int main() {
    LinkedList<int> listOne = parseLine(std::cin);
    LinkedList<int> listTwo = parseLine(std::cin);
    LinkedList<int> merged = merge_sorted(listOne, listTwo);

    std::size_t idx = 0;
    for (auto it = merged.begin(); it != merged.end(); ++it, ++idx) {
        if (idx > 0) std::cout << ' ';
        std::cout << *it;
    }
    std::cout << '\n';
    return 0;
}
