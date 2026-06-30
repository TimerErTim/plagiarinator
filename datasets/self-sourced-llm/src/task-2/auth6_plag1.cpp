#include <string>
#include <iostream>
#include <sstream>

template <typename T>
class LinkedList {
    struct Node {
        T value;
        Node* next;
        Node(const T& x) : value(x), next(nullptr) {}
    };

    Node* front_;
    std::size_t count_;

public:
    LinkedList() : front_(nullptr), count_(0) {}

    ~LinkedList() {
        Node* current = front_;
        while (current != nullptr) {
            Node* doomed = current;
            current = current->next;
            delete doomed;
        }
    }

    void push_back(const T& item) {
        Node* fresh = new Node(item);
        if (front_ == nullptr)
            front_ = fresh;
        else {
            Node* cursor = front_;
            while (cursor->next != nullptr)
                cursor = cursor->next;
            cursor->next = fresh;
        }
        count_++;
    }

    void emplace_back(const T& item) { push_back(item); }

    std::size_t size() const { return count_; }

    class Iterator {
        Node* node_;
    public:
        explicit Iterator(Node* n) : node_(n) {}
        T& operator*() { return node_->value; }
        const T& operator*() const { return node_->value; }
        Iterator& operator++() { node_ = node_->next; return *this; }
        Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }
        bool operator!=(const Iterator& other) const { return node_ != other.node_; }
    };

    Iterator begin() { return Iterator(front_); }
    Iterator end() { return Iterator(nullptr); }
    Iterator begin() const { return Iterator(front_); }
    Iterator end() const { return Iterator(nullptr); }
};

template <typename T>
LinkedList<T> merge_sorted(const LinkedList<T>& first, const LinkedList<T>& second) {
    LinkedList<T> combined;
    auto it1 = first.begin();
    auto it2 = second.begin();
    auto end1 = first.end();
    auto end2 = second.end();

    while (it1 != end1 && it2 != end2) {
        if (*it1 <= *it2) {
            combined.emplace_back(*it1);
            it1++;
        } else {
            combined.emplace_back(*it2);
            it2++;
        }
    }
    while (it1 != end1) { combined.emplace_back(*it1); ++it1; }
    while (it2 != end2) { combined.emplace_back(*it2); ++it2; }
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
            seq.emplace_back(token);
        return seq;
    }
}

int main() {
    LinkedList<int> seqA = io::readSequence(std::cin);
    LinkedList<int> seqB = io::readSequence(std::cin);
    LinkedList<int> merged = merge_sorted(seqA, seqB);

    bool firstElem = true;
    for (auto iter = merged.begin(); iter != merged.end(); iter++) {
        if (!firstElem) std::cout << ' ';
        std::cout << *iter;
        firstElem = false;
    }
    std::cout << '\n';
    return 0;
}
