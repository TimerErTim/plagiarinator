#include <iostream>
#include <sstream>
#include <string>

template <typename T>
class LinkedList
{
    struct Node
    {
        T value;
        Node* next;
        Node(const T& x) : value(x), next(nullptr) {}
    };

    Node* head_;
    std::size_t length_;

public:
    LinkedList() : head_(nullptr), length_(0) {}

    ~LinkedList()
    {
        Node* current = head_;
        while (current != nullptr)
        {
            Node* doomed = current;
            current = current->next;
            delete doomed;
        }
    }

    void push_back(const T& item)
    {
        Node* fresh = new Node(item);
        if (head_ == nullptr)
            head_ = fresh;
        else
        {
            Node* walker = head_;
            while (walker->next != nullptr)
                walker = walker->next;
            walker->next = fresh;
        }
        length_++;
    }

    std::size_t size() const { return length_; }

    class Iterator
    {
        Node* node_;
    public:
        explicit Iterator(Node* n) : node_(n) {}
        T& operator*() { return node_->value; }
        const T& operator*() const { return node_->value; }
        Iterator& operator++() { node_ = node_->next; return *this; }
        bool operator!=(const Iterator& other) const { return node_ != other.node_; }
    };

    Iterator begin() { return Iterator(head_); }
    Iterator end() { return Iterator(nullptr); }
    Iterator begin() const { return Iterator(head_); }
    Iterator end() const { return Iterator(nullptr); }
};

template <typename T>
LinkedList<T> merge_sorted(const LinkedList<T>& first, const LinkedList<T>& second)
{
    LinkedList<T> combined;
    auto it1 = first.begin();
    auto it2 = second.begin();
    auto end1 = first.end();
    auto end2 = second.end();

    while (it1 != end1 && it2 != end2)
    {
        if (*it1 <= *it2)
        {
            combined.push_back(*it1);
            ++it1;
        }
        else
        {
            combined.push_back(*it2);
            ++it2;
        }
    }
    while (it1 != end1) { combined.push_back(*it1); ++it1; }
    while (it2 != end2) { combined.push_back(*it2); ++it2; }
    return combined;
}

int main()
{
    LinkedList<int> seqA, seqB;
    for (int pass = 0; pass < 2; ++pass)
    {
        std::string buffer;
        std::getline(std::cin, buffer);
        std::istringstream lineStream(buffer);
        int token;
        LinkedList<int>& target = pass ? seqB : seqA;
        while (lineStream >> token)
            target.push_back(token);
    }

    LinkedList<int> merged = merge_sorted(seqA, seqB);

    for (std::size_t idx = 0; idx < merged.size(); ++idx)
    {
        if (idx > 0) std::cout << ' ';
        auto iter = merged.begin();
        for (std::size_t j = 0; j < idx; ++j) ++iter;
        std::cout << *iter;
    }
    std::cout << std::endl;
    return 0;
}
