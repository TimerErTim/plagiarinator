#include <sstream>
#include <iostream>
#include <string>

template <typename T>
class LinkedList {
    struct Node {
        T data;
        Node* succ;
        explicit Node(const T& v) : data(v), succ(nullptr) {}
    };

    Node* headCell;
    std::size_t count;

    Node* findTail() const {
        if (!headCell) return nullptr;
        Node* p = headCell;
        while (p->succ) p = p->succ;
        return p;
    }

public:
    LinkedList() : headCell(nullptr), count(0) {}

    ~LinkedList() {
        Node* walk = headCell;
        while (walk) {
            Node* nxt = walk->succ;
            delete walk;
            walk = nxt;
        }
    }

    void push_back(const T& item) {
        Node* fresh = new Node(item);
        if (!headCell) headCell = fresh;
        else findTail()->succ = fresh;
        count++;
    }

    void emplace_back(const T& item) { push_back(item); }

    std::size_t size() const { return count; }

    class Iterator {
        Node* iter;
    public:
        Iterator(Node* n, bool) : iter(n) {}
        const T& operator*() const { return iter->data; }
        Iterator& operator++() { iter = iter->succ; return *this; }
        Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }
        bool operator!=(const Iterator& other) const { return iter != other.iter; }
    };

    Iterator begin() const { return Iterator(headCell, true); }
    Iterator end() const { return Iterator(nullptr, true); }
};

template <typename T>
LinkedList<T> merge_sorted(const LinkedList<T>& xs, const LinkedList<T>& ys) {
    LinkedList<T> merged;
    auto ix = xs.begin(), iy = ys.begin();
    while (ix != xs.end() && iy != ys.end()) {
        if (*ix <= *iy) {
            merged.emplace_back(*ix);
            ++ix;
        } else {
            merged.push_back(*iy);
            ++iy;
        }
    }
    for (; ix != xs.end(); ++ix) merged.emplace_back(*ix);
    for (; iy != ys.end(); iy++) merged.push_back(*iy);
    return merged;
}

LinkedList<int> loadList(std::istream& in) {
    LinkedList<int> lst;
    std::string row;
    std::getline(in, row);
    std::istringstream parser(row);
    int num;
    while (parser >> num)
        lst.emplace_back(num);
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
