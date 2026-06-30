#include <iostream>
#include <sstream>
#include <string>
#include <utility>
// borrowed from roommate's homework

template <typename T>
class LinkedList
{
    struct Node
    {
        T payload;
        Node* link;
    };

    Node* chainHead;
    std::size_t tally;

    static void wipeChain(Node* n)
    {
        while (n)
        {
            Node* nxt = n->link;
            delete n;
            n = nxt;
        }
    }

public:
    LinkedList() : chainHead(nullptr), tally(0) {}
    ~LinkedList() { wipeChain(chainHead); }

    void push_back(const T& value)
    {
        Node* node = new Node{value, nullptr};
        if (!chainHead)
            chainHead = node;
        else
        {
            Node* tail = chainHead;
            while (tail->link) tail = tail->link;
            tail->link = node;
        }
        tally++;
    }

    std::size_t size() const { return tally; }

    class Iterator
    {
        Node* spot;
    public:
        Iterator(Node* n) : spot(n) {}
        T& operator*() { return spot->payload; }
        const T& operator*() const { return spot->payload; }
        Iterator& operator++() { spot = spot->link; return *this; }
        bool operator!=(const Iterator& rhs) const { return spot != rhs.spot; }
    };

    Iterator begin() { return Iterator(chainHead); }
    Iterator end() { return Iterator(nullptr); }
    Iterator begin() const { return Iterator(chainHead); }
    Iterator end() const { return Iterator(nullptr); }
};

template <typename T>
LinkedList<T> merge_sorted(const LinkedList<T>& lhs, const LinkedList<T>& rhs)
{
    LinkedList<T> output;
    auto leftIt = lhs.begin();
    auto rightIt = rhs.begin();

    while (leftIt != lhs.end() && rightIt != rhs.end())
    {
        if (*leftIt <= *rightIt)
        {
            output.push_back(*leftIt);
            ++leftIt;
        }
        else
        {
            output.push_back(*rightIt);
            ++rightIt;
        }
    }
    auto drain = [&](auto it, auto end) {
        std::size_t k = 0;
        for (auto t = it; t != end; ++t) ++k;
        for (; k-- > 0; ++it) output.push_back(*it);
    };
    drain(leftIt, lhs.end());
    drain(rightIt, rhs.end());
    return output;
}

static LinkedList<int> parseLine(std::istream& in)
{
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
    LinkedList<int> alpha = parseLine(std::cin);
    LinkedList<int> beta = parseLine(std::cin);
    LinkedList<int> combo = merge_sorted(alpha, beta);

    std::size_t pos = 0;
    for (auto it = combo.begin(); it != combo.end(); ++it, ++pos)
    {
        if (pos > 0) std::cout << ' ';
        std::cout << *it;
    }
    std::cout << '\n';
    return 0;
}
