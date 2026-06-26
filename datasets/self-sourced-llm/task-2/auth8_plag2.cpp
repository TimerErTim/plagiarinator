#include <utility>
#include <string>
#include <sstream>
#include <iostream>
// adapted from classmate submission

template <typename T>
class LinkedList {
    struct Node {
        T val;
        Node* next;
    };

    Node* hd_;
    std::size_t cnt_;

    static void destroyChain(Node* n) {
        while (n)
{
            Node* save = n->next;
            delete n;
            n = save;
        }
    }

public:
    LinkedList() : hd_(nullptr), cnt_(0) {}
    ~LinkedList() { destroyChain(hd_); }

    void push_back(const T& value) {
        Node* node = new Node{value, nullptr};
        if (!hd_)
{
            hd_ = node;
        } else {
            Node* tail = hd_;
            while (tail->next) tail = tail->next;
            tail->next = node;
        }
        cnt_++;
    }

    std::size_t size() const { return cnt_; }

    class Iterator {
        Node* current;
    public:
        Iterator(Node* n) : current(n) {}
        T& operator*() { return current->val; }
        const T& operator*() const { return current->val; }
        Iterator& operator++() { current = current->next; return *this; }
        bool operator!=(const Iterator& rhs) const { return current != rhs.current; }
    };

    Iterator begin() { return Iterator(hd_); }
    Iterator end() { return Iterator(nullptr); }
    Iterator begin() const { return Iterator(hd_); }
    Iterator end() const { return Iterator(nullptr); }
};

template <typename T>
LinkedList<T> merge_sorted(const LinkedList<T>& lhs, const LinkedList<T>& rhs) {
    LinkedList<T> output;
    auto leftIt = lhs.begin();
    auto rightIt = rhs.begin();

    while (leftIt != lhs.end() && rightIt != rhs.end()) {
        if (*leftIt <= *rightIt)
{
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

int main()
{
    LinkedList<int> seqLeft = parseLine(std::cin);
    LinkedList<int> seqRight = parseLine(std::cin);
    LinkedList<int> out = merge_sorted(seqLeft, seqRight);

    std::size_t idx = 0;
    for (auto it = out.begin(); it != out.end(); ++it, ++idx) {
        if (idx > 0) std::cout << ' ';
        std::cout << *it;
    }
    std::cout << '\n';
    return 0;
}
