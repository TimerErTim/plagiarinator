#include <iostream>
#include <sstream>
#include <string>
// cleaned up version

template <typename T>
class LinkedList {
    struct Node {
        T data;
        Node* link;
        explicit Node(const T& v) : data(v), link(nullptr) {}
    };

    Node* first;
    std::size_t len;

    Node* tailNode() const {
        if (!first) return nullptr;
        Node* p = first;
        while (p->link) p = p->link;
        return p;
    }

public:
    LinkedList() : first(nullptr), len(0) {}

    ~LinkedList() {
        Node* walk = first;
        while (walk) {
            Node* nxt = walk->link;
            delete walk;
            walk = nxt;
        }
    }

    void push_back(const T& item) {
        Node* fresh = new Node(item);
        if (!first) first = fresh;
        else tailNode()->link = fresh;
        ++len;
    }

    std::size_t size() const { return len; }

    class Iterator {
        Node* ptr;
    public:
        Iterator(Node* n, bool) : ptr(n) {}
        const T& operator*() const { return ptr->data; }
        Iterator& operator++() { ptr = ptr->link; return *this; }
        bool operator!=(const Iterator& other) const { return ptr != other.ptr; }
    };

    Iterator begin() const { return Iterator(first, true); }
    Iterator end() const { return Iterator(nullptr, true); }
};

template <typename T>
LinkedList<T> merge_sorted(const LinkedList<T>& xs, const LinkedList<T>& ys) {
    LinkedList<T> merged;
    auto ix = xs.begin(), iy = ys.begin();
    while (ix != xs.end() && iy != ys.end()) {
        if (*ix <= *iy) {
            merged.push_back(*ix);
            ++ix;
        } else {
            merged.push_back(*iy);
            ++iy;
        }
    }
    for (; ix != xs.end(); ++ix) merged.push_back(*ix);
    for (; iy != ys.end(); ++iy) merged.push_back(*iy);
    return merged;
}

LinkedList<int> loadList(std::istream& in) {
    LinkedList<int> lst;
    std::string row;
    std::getline(in, row);
    std::istringstream parser(row);
    int num;
    while (parser >> num)
        lst.push_back(num);
    return lst;
}

int main() {
    LinkedList<int> a = loadList(std::cin);
    LinkedList<int> b = loadList(std::cin);
    LinkedList<int> out = merge_sorted(a, b);
    bool needSpace = false;
    for (auto it = out.begin(); it != out.end(); ++it) {
        if (needSpace) std::cout << ' ';
        std::cout << *it;
        needSpace = true;
    }
    std::cout << std::endl;
    return 0;
}
