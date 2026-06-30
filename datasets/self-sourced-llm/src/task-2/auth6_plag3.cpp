#include <iostream>
#include <sstream>
#include <string>
// borrowed from roommate's homework

template <typename T>
class LinkedList
{
    struct Node
    {
        T payload;
        Node* link;
        Node(const T& x) : payload(x), link(nullptr) {}
    };

    Node* chainStart;
    std::size_t elemCount;

public:
    LinkedList() : chainStart(nullptr), elemCount(0) {}

    ~LinkedList()
    {
        Node* cur = chainStart;
        while (cur != nullptr)
        {
            Node* trash = cur;
            cur = cur->link;
            delete trash;
        }
    }

    void push_back(const T& item)
    {
        Node* fresh = new Node(item);
        if (chainStart == nullptr)
            chainStart = fresh;
        else
        {
            Node* rover = chainStart;
            while (rover->link != nullptr)
                rover = rover->link;
            rover->link = fresh;
        }
        elemCount++;
    }

    std::size_t size() const { return elemCount; }

    class Iterator
    {
        Node* spot;
    public:
        explicit Iterator(Node* n) : spot(n) {}
        T& operator*() { return spot->payload; }
        const T& operator*() const { return spot->payload; }
        Iterator& operator++() { spot = spot->link; return *this; }
        bool operator!=(const Iterator& peer) const { return spot != peer.spot; }
    };

    Iterator begin() { return Iterator(chainStart); }
    Iterator end() { return Iterator(nullptr); }
    Iterator begin() const { return Iterator(chainStart); }
    Iterator end() const { return Iterator(nullptr); }
};

template <typename T>
LinkedList<T> merge_sorted(const LinkedList<T>& alpha, const LinkedList<T>& beta)
{
    LinkedList<T> combo;
    auto ia = alpha.begin(), ib = beta.begin();
    auto ea = alpha.end(), eb = beta.end();

    while (ia != ea && ib != eb)
    {
        if (*ia <= *ib) { combo.push_back(*ia); ++ia; }
        else { combo.push_back(*ib); ++ib; }
    }
    auto flushRest = [&](auto it, auto end) {
        std::size_t n = 0;
        for (auto t = it; t != end; ++t) ++n;
        for (; n-- > 0; ++it) combo.push_back(*it);
    };
    flushRest(ia, ea);
    flushRest(ib, eb);
    return combo;
}

namespace io
{
    LinkedList<int> readSequence(std::istream& stream)
    {
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
    LinkedList<int> alpha = io::readSequence(std::cin);
    LinkedList<int> beta = io::readSequence(std::cin);
    LinkedList<int> combined = merge_sorted(alpha, beta);

    bool gapNeeded = true;
    for (auto step = combined.begin(); step != combined.end(); ++step)
    {
        if (!gapNeeded) std::cout << ' ';
        std::cout << *step;
        gapNeeded = false;
    }
    std::cout << '\n';
    return 0;
}
