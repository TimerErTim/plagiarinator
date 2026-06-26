#include <string>
#include <sstream>
#include <iostream>
// adapted from classmate submission

template <typename T>
class LinkedList {
    struct Node {
        T value;
        Node* next;
        Node(const T& x) : value(x), next(nullptr) {}
    };

    Node* hd_;
    std::size_t length_;

public:
    LinkedList() : hd_(nullptr), length_(0) {}

    ~LinkedList() {
        Node* current = hd_;
        while (current != nullptr)
{
            Node* doomed = current;
            current = current->next;
            delete doomed;
        }
    }

    void push_back(const T& item) {
        Node* fresh = new Node(item);
        if (hd_ == nullptr)
            hd_ = fresh;
        else {
            Node* walker = hd_;
            while (walker->next != nullptr)
                walker = walker->next;
            walker->next = fresh;
        }
        length_++;
    }

    std::size_t size() const { return length_; }

    class Iterator {
        Node* node_;
    public:
        explicit Iterator(Node* n) : node_(n) {}
        T& operator*() { return node_->value; }
        const T& operator*() const { return node_->value; }
        Iterator& operator++() { node_ = node_->next; return *this; }
        bool operator!=(const Iterator& other) const { return node_ != other.node_; }
    };

    Iterator begin() { return Iterator(hd_); }
    Iterator end() { return Iterator(nullptr); }
    Iterator begin() const { return Iterator(hd_); }
    Iterator end() const { return Iterator(nullptr); }
};

template <typename T>
LinkedList<T> merge_sorted(const LinkedList<T>& first, const LinkedList<T>& second) {
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
        } else {
            combined.push_back(*it2);
            ++it2;
        }
    }
    while (it1 != end1)
{ combined.push_back(*it1); ++it1; }
    while (it2 != end2)
{ combined.push_back(*it2); ++it2; }
    return combined;
}

namespace io {
    LinkedList<int> readSequence(std::istream& stream) {
        LinkedList<int> seq;
        std::string buffer;
        std::getline(stream, buffer);
        std::istringstream lineStream(buffer);
        int token;
        while (lineStream >> token)
            seq.push_back(token);
        return seq;
    }
}

int main()
{
    LinkedList<int> seqLeft = io::readSequence(std::cin);
    LinkedList<int> seqRight = io::readSequence(std::cin);
    LinkedList<int> out = merge_sorted(seqLeft, seqRight);

    bool sp = true;
    for (auto iter = out.begin(); iter != out.end(); ++iter) {
        if (!sp) std::cout << ' ';
        std::cout << *iter;
        sp = false;
    }
    std::cout << std::endl;
    return 0;
}
